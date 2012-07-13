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

using namespace json_spirit;

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

	NetworkPtr Serializer::ConvNetwork(serialize::Network& net){
		NetworkPtr sNet = Network::Create();
		sNet->SetNumIn(net.numIn);
		sNet->SetNumOut(net.numOut);
		sNet->SetNumHidLayers(net.numHidLayers);
		BOOST_FOREACH(serialize::Layer layer, net.layers){
			sNet->GetLayers().insert(LayerPair(layer.name,ConvLayer(layer)));
		}
		BOOST_FOREACH(serialize::Layer layer, net.layers){
			LayerPtr sLayer = sNet->GetLayer(layer.name);//change that to the get layer function.
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
		sNode.height = node->GetHeight();
		sNode.name = node->GetName();
		return sNode;
	}

	NodePtr Serializer::ConvNode(serialize::Node& node){
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
		sLayer.nextLayerName = layer->HasNextL() ? layer->GetNextLayer()->GetName() : "";
		sLayer.prevLayerName = layer->HasPrevL() ? layer->GetPrevLayer()->GetName() : "";
		sLayer.size	= layer->GetSize();
		sLayer.type	= layer->GetTypeName();
		BOOST_FOREACH(NodePtr node, layer->GetNodes()){
			sLayer.nodes.push_back(ConvNode(node));
		}

		return sLayer;
	}

	LayerPtr Serializer::ConvLayer (serialize::Layer& layer){
		LayerPtr sLayer = Layer::CreateLayer(Layer::hidden,layer.size,layer.height,layer.name);
		sLayer->SetTypeByName(layer.type);
		BOOST_FOREACH(serialize::Node node, layer.nodes){
			sLayer->AddNode(ConvNode(node),false);
		}
		return sLayer;
	}


	const mValue& JSONArchiver::findValue( const mObject& obj, const string& name  )
	{
		mObject::const_iterator i = obj.find( name );

		assert( i != obj.end() );
		assert( i->first == name );

		return i->second;
	}


	mObject JSONArchiver::writeNetwork( serialize::Network& net )
	{
		mObject newNet;
		newNet["numIn"] = net.numIn;
		newNet["numOut"] = net.numOut;
		newNet["numHidLayers"] = net.numHidLayers;
		newNet["layers"] = writeLayers(net.layers);
		newNet["connections"] = writeCons(net.connections);
		return newNet;
	}

	serialize::Network JSONArchiver::readNetwork( mObject& net )
	{
		serialize::Network newNet;
		newNet.numIn = findValue(net, "numIn").get_int();
		newNet.numOut = findValue(net, "numOut").get_int();
		newNet.numHidLayers = findValue(net, "numHidLayers").get_int();
		mArray cons = findValue(net, "connections").get_array();
		mArray layers = findValue(net, "layers").get_array();
		newNet.layers = readLayers(layers);
		newNet.connections = readCons(cons);
		return newNet;
	}

	mArray JSONArchiver::writeLayers( std::vector<serialize::Layer>& layers )
	{
		mArray newLayers;
		std::vector<serialize::Layer>::iterator it = layers.begin();
		for(; it != layers.end(); it++){
			newLayers.push_back(writeLayer((*it)));
		}
		return newLayers;
	}

	std::vector<serialize::Layer> JSONArchiver::readLayers( mArray& layers )
	{
		std::vector<serialize::Layer> newLayers;
		mArray::iterator it = layers.begin();
		for(; it != layers.end(); it++){
			newLayers.push_back(readLayer((*it).get_obj()));
		}
		return newLayers;
	}

	mObject JSONArchiver::writeLayer( serialize::Layer layer )
	{
		mObject newLayer;
		newLayer["type"] = layer.type;
		newLayer["size"] = layer.size;
		newLayer["height"] = layer.height;
		newLayer["name"] = layer.name;
		newLayer["prevLayerName"] = layer.prevLayerName;
		newLayer["nextLayerName"] = layer.nextLayerName;
		newLayer["nodes"] = writeNodes(layer.nodes);
		return newLayer;
	}

	serialize::Layer JSONArchiver::readLayer( mObject& layer )
	{
		serialize::Layer newLayer;
		newLayer.type = findValue(layer, "type").get_str();
		newLayer.size = findValue(layer, "size").get_int();
		newLayer.height = findValue(layer, "height").get_int();
		newLayer.name = findValue(layer, "name").get_str();
		newLayer.prevLayerName = findValue(layer, "prevLayerName").get_str();
		newLayer.nextLayerName = findValue(layer, "nextLayerName").get_str();
		mArray nodes = findValue(layer, "nodes").get_array();
		newLayer.nodes = readNodes(nodes);
		return newLayer;
	}

	mArray JSONArchiver::writeCons( std::vector<serialize::Connection>& cons )
	{
		mArray newCons;
		std::vector<serialize::Connection>::iterator it = cons.begin();
		for(; it != cons.end(); it++){
			newCons.push_back(writeCon((*it)));
		}
		return newCons;
	}

	std::vector<serialize::Connection> JSONArchiver::readCons( mArray& cons )
	{
		std::vector<serialize::Connection> newCons;
		mArray::iterator it = cons.begin();
		for (; it != cons.end(); it++){
			newCons.push_back(readCon((*it).get_obj()));
		}
		return newCons;
	}

	mObject JSONArchiver::writeCon( serialize::Connection& con )
	{
		mObject newCon;
		newCon["inNodeName"] = con.inNodeName;
		newCon["outNodeName"] = con.outNodeName;
		newCon["weight"] = con.weight;
		return newCon;
	}

	serialize::Connection JSONArchiver::readCon( mObject& con )
	{
		serialize::Connection newCon;
		newCon.inNodeName = findValue(con,"inNodeName").get_str();
		newCon.outNodeName = findValue(con, "outNodeName").get_str();
		newCon.weight = findValue(con, "weight").get_real();
		return newCon;
	}

	mObject JSONArchiver::writeNode( serialize::Node& node )
	{
		mObject newNode;
		newNode["name"] = node.name;
		newNode["height"] = node.height;
		newNode["activationFunc"] = node.activationFunc;
		return newNode;

	}

	serialize::Node JSONArchiver::readNode( mObject& node )
	{
		serialize::Node newNode;
		newNode.name = findValue(node, "name").get_str();
		newNode.height = findValue(node, "height").get_int();
		newNode.activationFunc = findValue(node, "activationFunc").get_str();
		return newNode;
	}

	std::vector<serialize::Node> JSONArchiver::readNodes( mArray& nodes )
	{
		std::vector<serialize::Node> newNodes;
		mArray::iterator it = nodes.begin();
		for(;it != nodes.end(); it++){
			newNodes.push_back(readNode((*it).get_obj()));
		}
		return newNodes;
	}

	mArray JSONArchiver::writeNodes( std::vector<serialize::Node>& nodes )
	{
		mArray newNodes;
		std::vector<serialize::Node>::iterator it = nodes.begin();
		for(;it != nodes.end();it++){
			newNodes.push_back(writeNode((*it)));
		}
		return newNodes;
	}

	NetworkPtr JSONArchiver::Load( istream& inStream )
	{
		mValue value;
		read_stream(inStream, value);
		mObject inNet = value.get_obj();
		serialize::Network sNet = readNetwork(inNet);
		NetworkPtr newNet = ConvNetwork(sNet);
		return newNet;
	}

	void JSONArchiver::Save( NetworkPtr net, ostream& stream)
	{
		serialize::Network sNet = ConvNetwork(net);
		mObject outNet = writeNetwork(sNet);
		write_stream(mValue(outNet),stream);
	}

}