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

	class Serializer {

	public:
		Serializer(){}
		~Serializer(){}
		virtual NetworkPtr Load(istream inStream) = 0;
		virtual ostream Save(NetworkPtr net) = 0;

	protected:
		static serialize::Network ConvNetwork(NetworkPtr net);
		static NetworkPtr ConvNetwork(serialize::Network& net);
		static serialize::Node ConvNode(NodePtr node);
		static NodePtr ConvNode(serialize::Node& node);
		static serialize::Connection ConvConnection (ConPtr con);
		static serialize::Layer ConvLayer (LayerPtr layer);
		static LayerPtr ConvLayer (serialize::Layer& layer);
	};

	class JSONArchiver : Serializer {
	public:
		virtual NetworkPtr Load(istream inStream);
		virtual ostream Save(NetworkPtr net);

	private:
		mObject writeNetwork(serialize::Network& net);
		serialize::Network readNetwork(mObject& obj);
	};
}