/* 
 * File:   layer.h
 * Author: jreeder
 *
 * Created on May 16, 2010, 8:35 PM
 */

#ifndef _LAYER_H
#define	_LAYER_H

#include "JRNN.h"

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
        void BuildLayer(Node::nodeType nType);

        NodeList& GetNodes();
        vecDouble GetOutput();

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

        int GetSize();

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

}
#endif	/* _LAYER_H */

