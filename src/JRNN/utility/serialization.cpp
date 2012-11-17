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
			sNet.connections.push_back(ConvConnection(conIT->second));
		}
		LayerMap::iterator layerIT = net->GetLayers().begin();
		LayerMap::iterator layerITend = net->GetLayers().end();
		for(;layerIT != layerITend; layerIT++){
			sNet.layers.push_back(ConvLayer(layerIT->second));
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
			LayerPtr sLayer = sNet->GetLayer(layer.name);
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

	void serialize::Network::ReadIn(NetworkPtr net){
		netType = Normal;
		numHidLayers = net->GetNumHidLayers();
		numIn = net->GetNumIn();
		numOut = net->GetNumOut();
		ConMap::iterator conIT = net->GetConnections().begin();
		ConMap::iterator conITend = net->GetConnections().end();
		for(;conIT != conITend; conIT++){
			connections.push_back(Serializer::ConvConnection(conIT->second));
		}
		LayerMap::iterator layerIT = net->GetLayers().begin();
		LayerMap::iterator layerITend = net->GetLayers().end();
		for(;layerIT != layerITend; layerIT++){
			layers.push_back(Serializer::ConvLayer(layerIT->second));
		}
	}

	void serialize::Network::WriteOut(NetworkPtr net){
		if (!net){
			assert(0);
		}
		net->SetNumIn(numIn);
		net->SetNumOut(numOut);
		net->SetNumHidLayers(numHidLayers);
		BOOST_FOREACH(serialize::Layer layer, layers){
			net->GetLayers().insert(LayerPair(layer.name,Serializer::ConvLayer(layer)));
		}
		BOOST_FOREACH(serialize::Layer layer, layers){
			LayerPtr sLayer = net->GetLayer(layer.name);
			if (layer.prevLayerName != ""){
				sLayer->SetPrevLayer(net->GetLayer(layer.prevLayerName));
			}
			if (layer.nextLayerName != ""){
				sLayer->SetNextLayer(net->GetLayer(layer.nextLayerName));
			}
		}
		BOOST_FOREACH(serialize::Connection con, connections){
			net->AddConnection(JRNN::Connection::Connect(net->GetNode(con.inNodeName), net->GetNode(con.outNodeName),con.weight));
		}
	}

	void serialize::CCNetwork::ReadIn(CCNetworkPtr net){
		Network::ReadIn(net);
		netType = CC;
		numUnits = net->GetNumUnits();
		candLayerName = net->GetCandLayer()->GetName();
		currentLayerName = net->GetCurrentLayer()->GetName();
		cloneOuts = net->GetCloneOuts();
		useSDCC = net->GetSDCC();
		varyActFunc = net->GetVaryActFunc();
		LayerList tmpHidLayers = net->GetHiddenLayers();
		LayerList::iterator it = tmpHidLayers.begin();
		for (; it != tmpHidLayers.end(); it++){
			hiddenLayerNames.push_back((*it)->GetName());
		}
	}

	void serialize::CCNetwork::WriteOut(CCNetworkPtr net){
		if (!net){
			assert(0);
		}
		Network::WriteOut(net);
		net->SetNumUnits(numUnits);
		net->SetCandLayerByName(candLayerName);
		net->SetCurrentLayerByName(currentLayerName);
		net->SetCloneOuts(cloneOuts);
		net->SetUseSDCC(useSDCC);
		net->SetVaryActFunc(varyActFunc);
		vector<string>::iterator it = hiddenLayerNames.begin();
		LayerList layerl = net->GetHiddenLayers();
		for (;it != hiddenLayerNames.end(); it++){
			layerl.push_back(net->GetLayer((*it)));
		}
	}

	void serialize::FFMLPNetwork::ReadIn(FFMLPNetPtr net){
		Network::ReadIn(net);
		netType = FFMLP;
	}

	void serialize::FFMLPNetwork::WriteOut(FFMLPNetPtr net){
		if (!net){
			assert(0);
		}
		Network::WriteOut(net);
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
		LayerPtr sLayer = Layer::CreateLayer(Layer::hidden,0,layer.height,layer.name); //The size will be incremented by adding nodes
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


	void JSONArchiver::writeNetwork( mObject& outNet, serialize::Network& net )
	{
		outNet["netType"] = net.netType;
		outNet["numIn"] = net.numIn;
		outNet["numOut"] = net.numOut;
		outNet["numHidLayers"] = net.numHidLayers;
		outNet["layers"] = writeLayers(net.layers);
		outNet["connections"] = writeCons(net.connections);
	}

	void JSONArchiver::readNetwork( serialize::Network& sNet, mObject& net )
	{
		sNet.netType = static_cast<serialize::NetType>(findValue(net, "netType").get_int());
		sNet.numIn = findValue(net, "numIn").get_int();
		sNet.numOut = findValue(net, "numOut").get_int();
		sNet.numHidLayers = findValue(net, "numHidLayers").get_int();
		mArray cons = findValue(net, "connections").get_array();
		mArray layers = findValue(net, "layers").get_array();
		sNet.layers = readLayers(layers);
		sNet.connections = readCons(cons);
	}

	void JSONArchiver::writeFFNetwork( mObject& outNet, serialize::FFMLPNetwork& net )
	{
		writeNetwork(outNet,net);
	}

	void JSONArchiver::readFFNetwork( serialize::FFMLPNetwork& sNet, mObject& net )
	{
		readNetwork(sNet,net);
	}

	void JSONArchiver::writeCCNetwork( mObject& outNet, serialize::CCNetwork& net )
	{
		//Basic things
		writeNetwork(outNet,net);

		//CC specific things. 
		outNet["numUnits"] = net.numUnits;
		outNet["candLayerName"] = net.candLayerName;
		outNet["currentLayerName"] = net.currentLayerName;
		outNet["hiddenLayerNames"] = writeStrings(net.hiddenLayerNames);
		outNet["cloneOuts"] = net.cloneOuts;
		outNet["useSDCC"] = net.useSDCC;
		outNet["varyActFunc"] = net.varyActFunc;
	}

	void JSONArchiver::readCCNetwork( serialize::CCNetwork& sNet, mObject& net )
	{
		//use network function for basic stuff
		readNetwork(sNet,net);

		//read in all the CC specific things. 
		sNet.numUnits = findValue(net, "numUnits").get_int();
		sNet.candLayerName = findValue(net, "candLayerName").get_str();
		sNet.currentLayerName = findValue(net, "currentLayerName").get_str();
		mArray hidLayerNames = findValue(net, "hiddenLayerNames").get_array();
		sNet.hiddenLayerNames = readStrings(hidLayerNames);
		sNet.cloneOuts = findValue(net, "cloneOuts").get_bool();
		sNet.useSDCC = findValue(net, "useSDCC").get_bool();
		sNet.varyActFunc = findValue(net, "varyActFunc").get_bool();
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

	json_spirit::mArray JSONArchiver::writeStrings( vector<string> hiddenLayerNames )
	{
		mArray newStrings;
		std::vector<string>::iterator it = hiddenLayerNames.begin();
		for (; it != hiddenLayerNames.end(); it++){
			newStrings.push_back((*it));
		}
		return newStrings;
	}

	vector<string> JSONArchiver::readStrings( mArray hidLayerNames )
	{
		std::vector<string> newStrings;
		mArray::iterator it = hidLayerNames.begin();
		for (; it != hidLayerNames.end(); it++){
			newStrings.push_back((*it).get_str());
		}
		return newStrings;
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

	NetworkPtr JSONArchiver::Load(istream& inStream )
	{
		mValue value;
		read_stream(inStream, value);
		mObject inNet = value.get_obj();
		NetworkPtr retNet;
		serialize::NetType netType = static_cast<serialize::NetType>(findValue(inNet, "netType").get_int());
		if (netType == serialize::Normal){
			serialize::Network sNet;
			readNetwork(sNet,inNet);
			retNet = Network::Create();
			sNet.WriteOut(retNet);
		}
		else if (netType == serialize::FFMLP){
			serialize::FFMLPNetwork sNetFF;
			readFFNetwork(sNetFF,inNet);
			FFMLPNetPtr tmpNet = FFMLPNetwork::Create();
			sNetFF.WriteOut(tmpNet);
			retNet = tmpNet;
		}
		else if (netType == serialize::CC){
			serialize::CCNetwork sNetCC;
			readCCNetwork(sNetCC,inNet);
			CCNetworkPtr tmpNetCC = CCNetwork::Create();
			sNetCC.WriteOut(tmpNetCC);
			retNet = tmpNetCC;
		}
		else {
			assert(0);
		}

		return retNet;
	}

	void JSONArchiver::Save( NetworkPtr inNet, ostream& outStream)
	{

		mObject outNet;

		if (typeid(CCNetwork) == typeid((*inNet))){
			serialize::CCNetwork sNet;
			sNet.ReadIn(dynamic_pointer_cast<CCNetwork>(inNet));
			writeCCNetwork(outNet,sNet);
		} 
		else if (typeid(FFMLPNetwork) == typeid((*inNet))){
			serialize::FFMLPNetwork sNet;
			sNet.ReadIn(dynamic_pointer_cast<FFMLPNetwork>(inNet));
			writeFFNetwork(outNet,sNet);
		}
		else {
			serialize::Network sNet;
			sNet.ReadIn(inNet);
			writeNetwork(outNet,sNet);	
		}

		write_stream(mValue(outNet),outStream);

		/*serialize::Network sNet = ConvNetwork(inNet);
		mObject outNet;
		writeNetwork(outNet, sNet);
		write_stream(mValue(outNet),outStream);*/
	}

	NetworkPtr JSONArchiver::LoadFromFile( string fileName )
	{
		ifstream ifile;
		ifile.open(fileName, ios_base::in);
		NetworkPtr net = this->Load(ifile);
		ifile.close();
		return net;
	}

	void JSONArchiver::SaveToFile( NetworkPtr inNet, string fileName )
	{
		ofstream ofile;
		ofile.open(fileName, ios_base::out);
		this->Save(inNet, ofile);
		ofile.close();
	}


	void DataSetArchiver::SaveDStoFile( DatasetPtr dataset, string filename )
	{
		ofstream ofile;
		ofile.open(filename, ios_base::out);

		ofile.close();
	}

	JRNN::DatasetPtr DataSetArchiver::ReadDSfromFile( string filename )
	{
		ifstream ifile;
		DatasetPtr ds(new Dataset());
		ifile.open(filename, ios_base::in);
		//TODO finish these functions
		ifile.close();
		return ds;
	}

	void DataSetArchiver::writeDataset( DatasetPtr dataset, mObject& retObj )
	{ 
		retObj["size"] = dataset->size;
		retObj["numInputs"] = dataset->numInputs;
		retObj["numOutputs"] = dataset->numOutputs;
		retObj["numTrain"] = dataset->numTrain;
		retObj["numVal"] = dataset->numVal;
		retObj["numTest"] = dataset->numTest;
		retObj["randSeed"] = dataset->randSeed;
		retObj["outputPerCategory"] = dataset->outputPerCategory;
		retObj["normalizeReals"] = dataset->normalizeReals;
		retObj["dsAnalyzed"] = dataset->dsAnalyzed;
		retObj["randomRange"] = JSONConverter::ConvertVector(dataset->randomRange);
		retObj["trainStdDev"] = dataset->trainStdDev;
		retObj["testStdDev"] = dataset->testStdDev;
		retObj["valStdDev"] = dataset->valStdDev;
		retObj["stdDev"] = dataset->stdDev;
		retObj["inputs"] = JSONConverter::ConvertMatDouble(dataset->inputs);
		retObj["outputs"] = JSONConverter::ConvertMatDouble(dataset->outputs);
		retObj["trainIns"] = JSONConverter::ConvertMatDouble(dataset->trainIns);
		retObj["trainOuts"] = JSONConverter::ConvertMatDouble(dataset->trainOuts);
		retObj["valIns"] = JSONConverter::ConvertMatDouble(dataset->valIns);
		retObj["valOuts"] = JSONConverter::ConvertMatDouble(dataset->valOuts);
		retObj["testIns"] = JSONConverter::ConvertMatDouble(dataset->testIns);
		retObj["testOuts"] = JSONConverter::ConvertMatDouble(dataset->testOuts);
		retObj["outClassIndexes"] = JSONConverter::ConvertHashedIntsMap(dataset->outClassIndexes);
		retObj["outClassPercentage"] = JSONConverter::ConvertHashedDoubleMap(dataset->outClassPercentage);
		retObj["outClassNames"] = JSONConverter::ConvertVector(dataset->outClassNames);
	}

	void DataSetArchiver::readDataset( mObject& dataset, DatasetPtr outDS )
	{
		outDS->size = dataset["size"].get_int();
		outDS->numInputs = dataset["numInputs"].get_int();
		outDS->numOutputs = dataset["numOutputs"].get_int();
		outDS->numTrain = dataset["numTrain"].get_int();
		outDS->numVal = dataset["numVal"].get_int();
		outDS->numTest = dataset["numTest"].get_int();
		outDS->randSeed = dataset["randSeed"].get_int();
		outDS->outputPerCategory = dataset["outputPerCategory"].get_bool();
		outDS->normalizeReals = dataset["normalizeReals"].get_bool();
		outDS->dsAnalyzed = dataset["dsAnalyzed"].get_bool();
		outDS->randomRange = JSONConverter::ConvertVector<ints>(dataset["randomRange"].get_array());
		outDS->trainStdDev = dataset["trainStdDev"].get_real();
		outDS->testStdDev = dataset["testStdDev"].get_real();
		outDS->valStdDev = dataset["valStdDev"].get_real();
		outDS->stdDev = dataset["stdDev"].get_real();
		outDS->inputs = JSONConverter::ConvertMatDouble(dataset["inputs"].get_array());
		outDS->outputs = JSONConverter::ConvertMatDouble(dataset["outputs"].get_array());
		outDS->trainIns = JSONConverter::ConvertMatDouble(dataset["trainIns"].get_array());
		outDS->trainOuts = JSONConverter::ConvertMatDouble(dataset["trainOuts"].get_array());
		outDS->valIns = JSONConverter::ConvertMatDouble(dataset["valIns"].get_array());
		outDS->valOuts = JSONConverter::ConvertMatDouble(dataset["valOuts"].get_array());
		outDS->testIns = JSONConverter::ConvertMatDouble(dataset["testIns"].get_array());
		outDS->testOuts = JSONConverter::ConvertMatDouble(dataset["testOuts"].get_array());
		outDS->outClassIndexes = JSONConverter::ConvertHashedIntsMap(dataset["outClassIndexes"].get_obj());
		outDS->outClassPercentage = JSONConverter::ConvertHashedDoubleMap(dataset["outClassPercentage"].get_obj());
		outDS->outClassNames = JSONConverter::ConvertVector<strings>(dataset["outClassNames"].get_array());
	}


	int JSONConverter::GetInt( mObject& obj, string name )
	{
		return findValue(obj, name).get_int();
	}

	json_spirit::mValue JSONConverter::findValue( const mObject& obj, const string& name )
	{
		mObject::const_iterator i = obj.find( name );

		assert( i != obj.end() );
		assert( i->first == name );

		return i->second;
	}

	bool JSONConverter::GetBool( mObject& obj, string name )
	{
		return findValue(obj, name).get_bool();
	}

	double JSONConverter::GetDouble( mObject& obj, string name )
	{
		return findValue(obj, name).get_real();
	}

	json_spirit::mArray JSONConverter::ConvertVecDouble( vecDouble& vec )
	{
		return ConvertVector(vec);
	}

	JRNN::vecDouble JSONConverter::ConvertVecDouble( mArray& vec )
	{
		//Can't do this the same as the other direction because the pyublas vecDouble doesn't have push_back
		//return ConvertVector<vecDouble>(vec);
		vecDouble retVec(vec.size());
		mArray::iterator it = vec.begin();
		vecDouble::iterator it2 = retVec.begin();
		for(;it != vec.end(); it++, it2++){
			(*it2) = (*it).get_real();
		}
		return retVec;
	}

	json_spirit::mArray JSONConverter::ConvertMatDouble( matDouble& mat )
	{
		matDouble::iterator it = mat.begin();
		mArray tmpMat;
		for(;it != mat.end(); it++){
			tmpMat.push_back(ConvertVector((*it)));
		}
		return tmpMat;
	}

	JRNN::matDouble JSONConverter::ConvertMatDouble( mArray& mat )
	{
		mArray::iterator it = mat.begin();
		matDouble tmpMat;
		for(;it != mat.end(); it++){
			tmpMat.push_back(ConvertVecDouble((*it).get_array()));
		}
		return tmpMat;
	}

	json_spirit::mObject JSONConverter::ConvertHashedIntsMap( Dataset::hashedIntsMap& intMap )
	{
		mObject tmpMap;
		Dataset::hashedIntsMap::iterator it = intMap.begin();
		for (; it != intMap.end(); it++){
			tmpMap[it->first] = ConvertVector(it->second);
		}
		return tmpMap;
	}

	Dataset::hashedIntsMap JSONConverter::ConvertHashedIntsMap( mObject& intMap )
	{
		Dataset::hashedIntsMap tmpMap;
		mObject::iterator it = intMap.begin();
		for(;it != intMap.end(); it++){
			tmpMap[it->first] = ConvertVector<ints>(it->second.get_array());
		}
		return tmpMap;
	}

	json_spirit::mObject JSONConverter::ConvertHashedDoubleMap( hashedDoubleMap& doubleMap )
	{
		mObject tmpMap;
		hashedDoubleMap::iterator it = doubleMap.begin();
		for(;it != doubleMap.end(); it++){
			tmpMap[it->first] = it->second;
		}
		return tmpMap;
	}

	JRNN::hashedDoubleMap JSONConverter::ConvertHashedDoubleMap( mObject& doubleMap )
	{
		hashedDoubleMap tmpMap;
		mObject::iterator it = doubleMap.begin();
		for(;it != doubleMap.end(); it++){
			tmpMap[it->first] = it->second.get_real();
		}
		return tmpMap;
	}

	template<class V>
	static mArray JSONConverter::ConvertVector(V& vec){
		V::iterator it = vec.begin();
		mArray tmpArray;
		for(;it != vec.end();it++){
			tmpArray.push_back(mValue((*it)));
		}
		return tmpArray;
	}

	template<class V>
	static V JSONConverter::ConvertVector(mArray& vec){
		mArray::iterator it = vec.begin();
		V tmpArray;
		typedef V::value_type value_type;
		for(;it != vec.end(); it++){
			tmpArray.push_back((*it).get_value<value_type>());
		}
		return tmpArray;
	}


}