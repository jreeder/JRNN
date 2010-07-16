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
        Layer(layerType type, int inLayerSize, int height, std::string name);
        Layer(const Layer& orig);
        virtual ~Layer();
        void Activate();
        void Activate(vecDouble inputs);
        //void BuildLayer(Node::nodeType nType);
		static LayerPtr CreateLayer(layerType type, int inLayerSize, int height, std::string name);
		template<class T>
		void BuildLayer();

        NodeList& GetNodes();
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
        layerType GetType() const;

        void SetName(std::string newName);
        const std::string& GetName();

		void AddNode(NodePtr node);
		void RemoveNode(NodePtr node);

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
        std::string name;
        LayerPtr prevLayer;
        LayerPtr nextLayer;
        NodeList nodes;
    };

	template<class T>
	void Layer::BuildLayer(){
		//NodePtr np;
		std::string baseName = name + "_";
		for (int i = 0; i < layerSize; i++){
			std::string num = lexical_cast<std::string>(i);
			std::string name = baseName + num;
			//np.reset(new Node<T>(height, name));
			nodes.push_back(Node::CreateNode<T>(height,name));
		}
	}
}
#endif	/* _LAYER_H */

