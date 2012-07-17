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
#include "networks/CCNetwork.h"
#include "networks/FFMLPNetwork.h"
#include <iostream>
#include <fstream>
#include <json_spirit_reader_template.h>
#include <json_spirit_writer_template.h>

using namespace std;
using namespace json_spirit;

namespace JRNN {

	namespace serialize {
		struct Node {
			string name;
			int height;
			string activationFunc;
			void ReadIn(NodePtr node);
			void WriteOut(NodePtr node);
		};

		struct Connection {
			string inNodeName;
			string outNodeName;
			double weight;
			void ReadIn(ConPtr con);
			//void WriteOut(ConPtr con); Not needed. 
		};

		struct Layer {
			string type;
			int size;
			int height;
			string name;
			string prevLayerName;
			string nextLayerName;
			std::vector<Node> nodes;
			void ReadIn(LayerPtr layer);
			void WriteOut(LayerPtr layer);
		};

		struct Network {
			int numIn;
			int numOut;
			int numHidLayers;
			std::vector<Layer> layers;
			std::vector<Connection> connections;
			virtual void ReadIn(NetworkPtr net);
			virtual void WriteOut(NetworkPtr net);
		};

		struct FFMLPNetwork : Network {
			virtual void ReadIn(FFMLPNetPtr net);
			virtual void WriteOut(FFMLPNetPtr net);
		};

		struct CCNetwork : Network {
			int numUnits;
			string candLayerName;
			string currentLayerName;
			vector<string> hiddenLayerNames;
			bool cloneOuts;
			bool useSDCC;
			bool varyActFunc;
			virtual void ReadIn(CCNetworkPtr net);
			virtual void WriteOut(CCNetworkPtr net);
		};
	}

	class Serializer {

	public:
		Serializer(){}
		~Serializer(){}
		virtual NetworkPtr Load(istream& inStream) = 0;
		virtual void Save(NetworkPtr net, ostream& stream) = 0;

	protected:
		/*static serialize::FFMLPNetwork ConvFFMLPNetwork(FFMLPNetPtr net);
		static FFMLPNetPtr ConvFFMLPNetwork(serialize::FFMLPNetwork& net);
		static serialize::CCNetwork ConvCCNetwork(CCNetworkPtr net);
		static CCNetworkPtr ConvCCNetwork(serialize::CCNetwork& net);*/
		static serialize::Network ConvNetwork(NetworkPtr net);
		static NetworkPtr ConvNetwork(serialize::Network& net);
		static serialize::Node ConvNode(NodePtr node);
		static NodePtr ConvNode(serialize::Node& node);
		static serialize::Connection ConvConnection (ConPtr con);
		static serialize::Layer ConvLayer (LayerPtr layer);
		static LayerPtr ConvLayer (serialize::Layer& layer);
	};

	class JSONArchiver : public Serializer {
	public:
		virtual NetworkPtr Load(istream& inStream);
		virtual void Save(NetworkPtr net, ostream& stream);

	private:
		const mValue& findValue( const mObject& obj, const string& name  );
		mObject writeNetwork(serialize::Network& net);
		serialize::Network readNetwork(mObject& obj);
		mArray writeLayers(std::vector<serialize::Layer>& layers);
		std::vector<serialize::Layer> readLayers(mArray& layers);
		mObject writeLayer(serialize::Layer layer);
		serialize::Layer readLayer(mObject& layer);
		mArray writeCons(std::vector<serialize::Connection>& cons);
		std::vector<serialize::Connection> readCons(mArray& cons);
		mObject writeCon(serialize::Connection& con);
		serialize::Connection readCon(mObject& con);
		mObject writeNode(serialize::Node& node);
		serialize::Node readNode(mObject& node);
		std::vector<serialize::Node> readNodes(mArray& nodes);
		mArray writeNodes(std::vector<serialize::Node>& nodes);
		
	};
}