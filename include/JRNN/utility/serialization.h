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

	class Serializer
	{
	public:
		static serialize::Network FillNetwork(NetworkPtr net);
		static NetworkPtr FillNetwork(serialize::Network net);

	protected:
		static serialize::Node FillNode(NodePtr node);
		static NodePtr FillNode(serialize::Node node);
		static serialize::Connection FillConnection (ConPtr con);
		static serialize::Layer FillLayer (LayerPtr layer);
		static LayerPtr FillLayer (serialize::Layer layer);

	private:
		Serializer();
		~Serializer();
	};
}