/* 
 * File:   layer.h
 * Author: jreeder
 *
 * Created on May 16, 2010, 8:35 PM
 */

#ifndef _LAYER_H
#define	_LAYER_H

#include "JRNN.h"
#include "structure/node.h"

namespace JRNN {
    
	class Layer;
	typedef boost::shared_ptr<Layer> LayerPtr;
	typedef std::map<std::string,LayerPtr> LayerMap;
	typedef std::pair<std::string, LayerPtr> LayerPair;
	typedef std::vector<LayerPtr> LayerList;


    class Layer {
    public:
        
        enum layerType {
            input,
            hidden,
            out,
            bias,
			spec
        };

        Layer();
        Layer(layerType type, int inLayerSize, int height, string name, bool shallow = false);
        Layer(const Layer& orig);
        virtual ~Layer();
        void Activate();
        void Activate(vecDouble inputs);
        //void BuildLayer(Node::nodeType nType);
		static LayerPtr CreateLayer(layerType type, int inLayerSize, int height, string name, bool shallow = false);
		static LayerPtr Clone( LayerPtr layer );
		void ShallowCopy (LayerPtr layer);
		template<class T>
		void BuildLayer();

        NodeList& GetNodes();
		NodePtr GetNodeByName( string name );
        vecDouble GetOutput();

		ConList GetConnections();

        //getters/setters
        void SetNextLayer(LayerPtr nextLayer);
        LayerPtr GetNextLayer() const;
		bool HasNextL();

        void SetPrevLayer(LayerPtr prevLayer);
        LayerPtr GetPrevLayer() const;
		bool HasPrevL();

        void SetLayerSize(int layerSize);
        int GetLayerSize() const;

        void SetType(layerType type);
		void SetTypeByName(string type);
        layerType GetType() const;
		string GetTypeName();

        void SetName(string newName);
        const string& GetName();
		NodeList ResetNodeNames();

		void AddNode(NodePtr node, bool shallow = false);
		//void AddNode( NodePtr node, bool createName);
		void InsertNode(NodePtr node, int pos, bool shallow = false);
		void RemoveNode(NodePtr node);
		int RemoveUnconnectedNodes();

		void Clear(bool disconnect = true);

        int GetSize();
		int GetHeight();
		void SetHeight(int newHeight);
		vecDouble GetPrimes();
		bool GetShallowLayer() const;
		void SetShallowLayer(bool val);

	private:
        layerType type;
        //vector<double> outBuffer;
        int layerSize;
        int height;
        string name;
        LayerPtr prevLayer;
        LayerPtr nextLayer;
        NodeList nodes;
		bool shallowLayer;
    };

	template<class T>
	void Layer::BuildLayer(){
		//NodePtr np;
		string baseName = name + "_";
		for (int i = 0; i < layerSize; i++){
			string num = lexical_cast<string>(i);
			string name = baseName + num;
			//np.reset(new Node<T>(height, name));
			nodes.push_back(Node::CreateNode<T>(height,name));
		}
	}
}
#endif	/* _LAYER_H */

