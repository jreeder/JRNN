/********************************************************************
	created:	2011/04/11
	created:	11:4:2011   18:14
	file base:	serialization
	file ext:	h
	author:		jreeder
	
	purpose:	Static utility class to serialize and de-serialize the 
	neural networks
*********************************************************************/

#include "JRNN.h"
#include "structure/network.h"

namespace JRNN {

	namespace serialization {
		struct Node {
			string name;
			int height;
			string activationFunc;
		};

		struct Connection {
			string inNodeName;
			string outNodeName;
			double weight;
		};

		struct Layer {
			string type;
			int size;
			int height;
			string name;
			string prevLayerName;
			string nextLayerName;
			std::vector<Node> Nodes;
		};

		struct Network {
			int numIn;
			int numOut;
			int numHidLayers;
			std::vector<Layer> layers;
			std::vector<Connection> connections;
		};
	}
	serialization::Node FillNode(NodePtr node){
		serialization::Node sNode;
		sNode.activationFunc = node->GetActFuncType();
		sNode.height = node->getHeight();
		sNode.name = node->GetName();
		return sNode;
	}

	serialization::Connection FillConnection (ConPtr con){
		serialization::Connection sCon;
		sCon.weight = con->GetWeight();
		sCon.inNodeName = con->GetInNodeName();
		sCon.outNodeName = con->GetOutNodeName();
		return sCon;
	}

	serialization::Layer FillLayer (LayerPtr layer){
		serialization::Layer sLayer;
		sLayer.height = layer->GetHeight();
		sLayer.name = layer->GetName();
		sLayer.nextLayerName = layer->GetNextLayer()->GetName();
		sLayer.prevLayerName = layer->GetPrevLayer()->GetName();
		sLayer.size	= layer->GetSize();
		sLayer.type	= layer->GetTypeName();
		BOOST_FOREACH(NodePtr node, layer->GetNodes()){
			sLayer.Nodes.push_back(FillNode(node));
		}
		return sLayer;
	}

	serialization::Network FillNetwork(NetworkPtr net){
		serialization::Network sNet;
		sNet.numHidLayers = net->GetNumHidLayers();
		sNet.numIn = net->GetNumIn();
		sNet.numOut = net->GetNumOut();
		ConMap::iterator conIT = net->GetConnections().begin();
		ConMap::iterator conITend = net->GetConnections().end();
		for(;conIT != conITend; conIT++){
			ConPair con = (*conIT);
			sNet.connections.push_back(FillConnection(con.second));
		}
		LayerMap::iterator layerIT = net->GetLayers().begin();
		LayerMap::iterator layerITend = net->GetLayers().end();
		for(;layerIT != layerITend; layerIT++){
			sNet.layers.push_back(FillLayer((*layerIT).second));
		}
		return sNet;
	}
}