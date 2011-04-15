/********************************************************************
	created:	2011/04/13
	created:	13:4:2011   15:04
	file base:	serialization
	file ext:	cpp
	author:		jreeder
	
	purpose:	Implementation of the serialization classes. 
*********************************************************************/

#include "JRNN.h"
#include "utility/serialization.h"

namespace JRNN {

	serialize::Network Serializer::ConvNetwork(NetworkPtr net){
		serialize::Network sNet;
		sNet.numHidLayers = net->GetNumHidLayers();
		sNet.numIn = net->GetNumIn();
		sNet.numOut = net->GetNumOut();
		ConMap::iterator conIT = net->GetConnections().begin();
		ConMap::iterator conITend = net->GetConnections().end();
		for(;conIT != conITend; conIT++){
			ConPair con = (*conIT);
			sNet.connections.push_back(ConvConnection(con.second));
		}
		LayerMap::iterator layerIT = net->GetLayers().begin();
		LayerMap::iterator layerITend = net->GetLayers().end();
		for(;layerIT != layerITend; layerIT++){
			sNet.layers.push_back(ConvLayer((*layerIT).second));
		}
		return sNet;
	}

	NetworkPtr Serializer::ConvNetwork(serialize::Network net){
		NetworkPtr sNet = Network::Create();
		sNet->SetNumIn(net.numIn);
		sNet->SetNumOut(net.numOut);
		sNet->SetNumHidLayers(net.numHidLayers);
		BOOST_FOREACH(serialize::Layer layer, net.layers){
			sNet->GetLayers().insert(LayerPair(layer.name,ConvLayer(layer)));
		}
		BOOST_FOREACH(serialize::Layer layer, net.layers){
			LayerPtr sLayer = sNet->GetLayers()[layer.name];
			if (layer.prevLayerName != ""){
				sLayer->SetPrevLayer(sNet->GetLayer(layer.prevLayerName));
			}
			if (layer.nextLayerName != ""){
				sLayer->SetNextLayer(sNet->GetLayer(layer.nextLayerName));
			}
		}
		BOOST_FOREACH(serialize::Connection con, net.connections){
			sNet->AddConnection(Connection::Connect(sNet->GetNode(con.inNodeName), sNet->GetNode(con.outNodeName),con.weight));
		}
		return sNet;
	}

	serialize::Node Serializer::ConvNode(NodePtr node){
		serialize::Node sNode;
		sNode.activationFunc = node->GetActFuncType();
		sNode.height = node->getHeight();
		sNode.name = node->GetName();
		return sNode;
	}

	NodePtr Serializer::ConvNode(serialize::Node node){
		NodePtr sNode;
		if (node.activationFunc == "SIGMOID"){
			sNode = Node::CreateNode<Sigmoid>(node.height,node.name);
		}
		else if(node.activationFunc == "ASIGMOID"){
			sNode = Node::CreateNode<ASigmoid>(node.height,node.name);
		}
		else if(node.activationFunc == "GAUSSIAN"){
			sNode = Node::CreateNode<Gaussian>(node.height,node.name);
		}
		else if(node.activationFunc == "BIAS"){
			sNode = Node::CreateNode<Bias>(node.height,node.name);
		}
		else {
			sNode = Node::CreateNode<Linear>(node.height,node.name);
		}
		return sNode;
	}

	serialize::Connection Serializer::ConvConnection (ConPtr con){
		serialize::Connection sCon;
		sCon.weight = con->GetWeight();
		sCon.inNodeName = con->GetInNodeName();
		sCon.outNodeName = con->GetOutNodeName();
		return sCon;
	}

	serialize::Layer Serializer::ConvLayer (LayerPtr layer){
		serialize::Layer sLayer;
		sLayer.height = layer->GetHeight();
		sLayer.name = layer->GetName();
		sLayer.nextLayerName = layer->GetNextLayer()->GetName();
		sLayer.prevLayerName = layer->GetPrevLayer()->GetName();
		sLayer.size	= layer->GetSize();
		sLayer.type	= layer->GetTypeName();
		BOOST_FOREACH(NodePtr node, layer->GetNodes()){
			sLayer.Nodes.push_back(ConvNode(node));
		}

		return sLayer;
	}

	LayerPtr Serializer::ConvLayer (serialize::Layer layer){
		LayerPtr sLayer = Layer::CreateLayer(Layer::hidden,layer.size,layer.height,layer.name);
		sLayer->SetTypeByName(layer.type);
		BOOST_FOREACH(serialize::Node node, layer.Nodes){
			sLayer->AddNode(ConvNode(node),false);
		}
		return sLayer;
	}
}