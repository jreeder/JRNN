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
    
    class Layer {
    public:
        
        enum layerType {
            input,
            hidden,
            out,
            bias
        };

        Layer();
        Layer(layerType type, int inLayerSize, int height, string name);
        Layer(const Layer& orig);
        virtual ~Layer();
        void Activate();
        void Activate(vecDouble inputs);
        //void BuildLayer(Node::nodeType nType);
		static LayerPtr CreateLayer(layerType type, int inLayerSize, int height, string name);
		static LayerPtr Clone( LayerPtr layer );
		template<class T>
		void BuildLayer();

        NodeList& GetNodes();
		NodePtr GetNodeByName( string name );
        vecDouble GetOutput();

		ConList GetConnections();

        //getters/setters
        void SetNextLayer(LayerPtr nextLayer);
        LayerPtr GetNextLayer() const;

        void SetPrevLayer(LayerPtr prevLayer);
        LayerPtr GetPrevLayer() const;

        void SetLayerSize(int layerSize);
        int GetLayerSize() const;

        void SetType(layerType type);
		void SetTypeByName(string type);
        layerType GetType() const;
		string GetTypeName();

        void SetName(string newName);
        const string& GetName();

		void AddNode(NodePtr node);
		void RemoveNode(NodePtr node);
		int RemoveUnconnectedNodes();

		void Clear();

        int GetSize();
		int GetHeight();
		void SetHeight(int newHeight);
		vecDouble GetPrimes();

	private:
        layerType type;
        //vector<double> outBuffer;
        int layerSize;
        int height;
        string name;
        LayerPtr prevLayer;
        LayerPtr nextLayer;
        NodeList nodes;
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

