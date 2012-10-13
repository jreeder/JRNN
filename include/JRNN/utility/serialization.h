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
#include "utility/dataset.h"
#include "utility/csmtldataset.h"
#include <iostream>
#include <fstream>
#include <json_spirit_reader_template.h>
#include <json_spirit_writer_template.h>
#include <typeinfo>

using namespace std;
using namespace json_spirit;

namespace JRNN {

	namespace serialize {

		enum NetType {
			Normal,
			FFMLP,
			CC
		};

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
			std::vector<Node> nodes;
		};

		struct Network {
			NetType netType;
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
		virtual void Save(NetworkPtr inNet, ostream& outStream) = 0;
		friend struct serialize::Network;

	protected:
		/*static serialize::FFMLPNetwork ConvFFMLPNetwork(FFMLPNetPtr net);
		static FFMLPNetPtr ConvFFMLPNetwork(serialize::FFMLPNetwork& net);
		static serialize::CCNetwork ConvCCNetwork(CCNetworkPtr net);
		static CCNetworkPtr ConvCCNetwork(serialize::CCNetwork& net);*/
		static serialize::Network ConvNetwork(NetworkPtr net); //Not used anymore
		static NetworkPtr ConvNetwork(serialize::Network& net); //Not used anymore
		static serialize::Node ConvNode(NodePtr node);
		static NodePtr ConvNode(serialize::Node& node);
		static serialize::Connection ConvConnection (ConPtr con);
		static serialize::Layer ConvLayer (LayerPtr layer);
		static LayerPtr ConvLayer (serialize::Layer& layer);
	};

	class JSONArchiver : public Serializer {
	public:
		virtual NetworkPtr Load(istream& inStream);
		virtual void Save(NetworkPtr inNet, ostream& outStream);
		NetworkPtr LoadFromFile(string fileName);
		void SaveToFile(NetworkPtr inNet, string fileName);

	private:
		const mValue& findValue( const mObject& obj, const string& name  );
		//Overloaded functions for the different types of networks
		void writeNetwork(mObject& outNet, serialize::Network& net);
		void readNetwork(serialize::Network& sNet, mObject& net);
		void writeFFNetwork(mObject& outNet, serialize::FFMLPNetwork& net);
		void readFFNetwork(serialize::FFMLPNetwork& sNet, mObject& net);
		void writeCCNetwork(mObject& outNet, serialize::CCNetwork& net);
		void readCCNetwork(serialize::CCNetwork& sNet, mObject& net);

		mArray writeLayers(std::vector<serialize::Layer>& layers);
		std::vector<serialize::Layer> readLayers(mArray& layers);
		mObject writeLayer(serialize::Layer layer);
		serialize::Layer readLayer(mObject& layer);
		mArray writeCons(std::vector<serialize::Connection>& cons);
		std::vector<serialize::Connection> readCons(mArray& cons);
		mObject writeCon(serialize::Connection& con);
		serialize::Connection readCon(mObject& con);
		mArray writeStrings( vector<string> hiddenLayerNames );
		std::vector<string> readStrings( mArray hidLayerNames );
		mObject writeNode(serialize::Node& node);
		serialize::Node readNode(mObject& node);
		std::vector<serialize::Node> readNodes(mArray& nodes);
		mArray writeNodes(std::vector<serialize::Node>& nodes);
		
	};

	class JSONConverter {
	public:
		//Basic Find Value
		static mValue findValue( const mObject& obj, const string& name  );

		//Grab Basic types from an Object. The reverse is trivial
		static int GetInt(mObject& obj, string name);
		static bool GetBool(mObject& obj, string name);
		static double GetDouble(mObject& obj, string name);

		//More complicated conversions
		template<class V>
		static mArray ConvertVector(V& vec);
		template<class V>
		static V ConvertVector(mArray& vec);

		static mArray ConvertVecDouble(vecDouble& vec);
		static vecDouble ConvertVecDouble(mArray& vec);

		static mArray ConvertMatDouble(matDouble& mat);
		static matDouble ConvertMatDouble(mArray& mat);

		static mObject ConvertHashedIntsMap(Dataset::hashedIntsMap& intMap);
		static Dataset::hashedIntsMap ConvertHashedIntsMap(mObject& intMap);

		static mObject ConvertHashedDoubleMap(hashedDoubleMap& doubleMap);
		static hashedDoubleMap ConvertHashedDoubleMap(mObject& doubleMap);

	};

	class DataSetArchiver {

	public:
		void SaveDStoFile(DatasetPtr dataset, string filename);
		DatasetPtr ReadDSfromFile(string filename);

	private:
		void writeDataset(DatasetPtr dataset, mObject& retObj);
		void readDataset(mObject& dataset, DatasetPtr outDS);
		void writeCSMTLDataset(CSMTLDatasetPtr dataset, mObject& retObj);
		void readCSMTLDataset(mObject& dataset, CSMTLDatasetPtr outDS);
	};
}