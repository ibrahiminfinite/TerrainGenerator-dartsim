
#include <dart/dart.hpp>
#include <dart/gui/osg/osg.hpp>
#include "dartUtils/HeightMapWidget.h"
#include "dartUtils/HeightMapTerrain.h"
#include "TerrainGenerator.h"



class HeightmapWorld : public gui::osg::WorldNode
{
public:
    explicit HeightmapWorld(simulation::WorldPtr world)
            : gui::osg::WorldNode(std::move(world))
    {
        // Do nothing
    }

    // Triggered at the beginning of each simulation step
    void customPreStep() override
    {
        // Do nothing
    }

protected:
};


int main() {


    dart::gui::osg::ImGuiViewer viewer;

    viewer.setUpViewInWindow(0, 0, 1280, 720);
    viewer.getCameraManipulator()->setHomePosition(
            ::osg::Vec3(2.57f, 3.14f, 1.64f),
            ::osg::Vec3(0.00f, 0.00f, 0.30f),
            ::osg::Vec3(-0.24f, -0.25f, 0.94f));
    viewer.setCameraManipulator(viewer.getCameraManipulator());


    TerrainGenerator generator;
    TerrainConfig config;

    config.terrainType = TerrainType::Plane;
    config.xSize = config.ySize = 8.0;
    config.resolution = 0.2;


    // Hills
    config.resolution = 0.2;
    config.amplitude = 1.0;
    config.frequency = 0.2;
    config.roughenss = 0.001;
    config.numOctaves = 1;

    // Steps
    config.resolution = 0.02;
    config.stepWidth = 0.2;
    config.stepHeight = 0.1;


    auto world = dart::simulation::World::create();
    world->setGravity(Eigen::Vector3d::Zero());


    // Create an instance of our customized WorldNode
    ::osg::ref_ptr<HeightmapWorld> node = new HeightmapWorld(world);
    node->setNumStepsPerCycle(16);

    auto terrainFrame = createHeightmapFrame<float>(generator, config);
    world->addSimpleFrame(terrainFrame);

    viewer.addWorldNode(node);
    viewer.simulate(true);


    // Create grid
    ::osg::ref_ptr<gui::osg::GridVisual> grid = new gui::osg::GridVisual();

    viewer.addAttachment(grid);

    viewer.getImGuiHandler()->addWidget(std::make_shared<Widget>(&viewer, config, generator, terrainFrame));
    viewer.run();

}
