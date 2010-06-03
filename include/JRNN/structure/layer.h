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
    
    class layer {
    public:
        
        enum layerType {
            input,
            hidden,
            out,
            bias
        };

        layer();
        layer(layerType type, int inLayerSize, int height, std::string name);
        layer(const layer& orig);
        virtual ~layer();
        void activate();
        void activate(vecDouble inputs);
        void buildLayer(node::nodeType nType);

        nodeList& getNodes();
        vecDouble getOutput();

        //getters/setters
        void setNextLayer(layerPtr nextLayer);
        layerPtr getNextLayer() const;

        void setPrevLayer(layerPtr prevLayer);
        layerPtr getPrevLayer() const;

        void setLayerSize(int layerSize);
        int getLayerSize() const;

        void setType(layerType type);
        layerType getType() const;

        void setName(std::string newName);
        const std::string& getName();

        int getSize();

    private:
        layerType type;
        //vector<double> outBuffer;
        int layerSize;
        int height;
        std::string name;
        layerPtr prevLayer;
        layerPtr nextLayer;
        nodeList nodes;
    };

}
#endif	/* _LAYER_H */

