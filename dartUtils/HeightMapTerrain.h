#ifndef TERRAINGENERATOR_DART_HEIGHTMAPTERRAIN_H
#define TERRAINGENERATOR_DART_HEIGHTMAPTERRAIN_H

#include <dart/dart.hpp>
#include "TerrainGenerator.h"

using namespace dart;


template <typename S>
dynamics::ShapePtr createHeightmapShape(TerrainGenerator& generator,
                                        TerrainConfig& config)
{
    if(config.terrainType == TerrainType::Inavlid)
    {
        dterr << "Invalid Terrain Type"<< std::endl;
    }

    using Vector3 = Eigen::Matrix<S, 3, 1>;

    Terrain terrain = generator.generate(config);

    auto scale = Vector3(config.resolution,config.resolution, 1);

    auto terrainShape = std::make_shared<dynamics::HeightmapShape<S>>();
    terrainShape->setScale(scale);
    auto xs = int(config.xSize / config.resolution) + 1;
    auto ys = int(config.ySize / config.resolution) + 1;
    std::cout<<" XS : "<<xs << " YS : "<<ys<<" HS : "<<terrain.heights.size()<<std::endl;

    terrainShape->setHeightField(xs,
                                 ys,
                                 terrain.heights);

    return terrainShape;
}

template <typename S>
dynamics::SimpleFramePtr createHeightmapFrame(TerrainGenerator& generator,
                                              TerrainConfig& config)
{
    auto terrainFrame = dynamics::SimpleFrame::createShared(dynamics::Frame::World());
    auto tf = terrainFrame->getRelativeTransform();
    tf.translation()[0] = -static_cast<double>(config.xSize) / 2.0;
    tf.translation()[1] = +static_cast<double>(config.ySize) / 2.0;
    terrainFrame->setRelativeTransform(tf);

    terrainFrame->createVisualAspect();

    auto terrainShape = createHeightmapShape<float>(generator,config);
    terrainFrame->setShape(terrainShape);

    return terrainFrame;
}



#endif //TERRAINGENERATOR_DART_HEIGHTMAPTERRAIN_H
