//
// Created by ibrahim on 22/03/22.
//

#ifndef TERRAINGENERATOR_HEIGHTMAPWIDGET_H
#define TERRAINGENERATOR_HEIGHTMAPWIDGET_H

#include <algorithm>
#include <dart/gui/osg/osg.hpp>
#include <dart/common/Console.hpp>
#include <dart/external/imgui/imgui.h>
#include "TerrainGenerator.h"

using namespace dart;


class Widget : public gui::osg::ImGuiWidget
{

public:
    Widget(
            gui::osg::ImGuiViewer* viewer,
            TerrainConfig& config,
            TerrainGenerator& generator,
            dynamics::SimpleFramePtr terrainFrame
    ):mViewer(viewer),
    mTerrain(std::move(terrainFrame)),
    config_(config),
    generator_(generator)
    {
        // do nothing
    }



    void render() override
    {
        ImGui::SetNextWindowPos(ImVec2(10, 20));
        ImGui::SetNextWindowSize(ImVec2(360, 350));
        ImGui::SetNextWindowBgAlpha(0.5f);

        if (!ImGui::Begin(
                "Heightmap Demo",
                nullptr,
                ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_HorizontalScrollbar))
        {
            // Early out if the window is collapsed, as an optimization.
            ImGui::End();
            return;
        }


        if (ImGui::CollapsingHeader("Simulation", ImGuiTreeNodeFlags_DefaultOpen))
        {
            int e = mViewer->isSimulating() ? 0 : 1;
            if (mViewer->isAllowingSimulation())
            {
                if (ImGui::RadioButton("Play", &e, 0) && !mViewer->isSimulating())
                    mViewer->simulate(true);
                ImGui::SameLine();
                if (ImGui::RadioButton("Pause", &e, 1) && mViewer->isSimulating())
                    mViewer->simulate(false);
            }
        }

        if (ImGui::CollapsingHeader("Terrain", ImGuiTreeNodeFlags_DefaultOpen))
        {

            // Terrain roughness
            if (ImGui::SliderFloat("Roughness", &config_.roughenss, 0.001, 0.05,"%.3f"))
            {
                updateHeightmapShape();
            }

            // Noise amplitude
            if (ImGui::SliderFloat("Amplitude", &config_.amplitude, 0.2, 3.0,"%.3f"))
            {
                updateHeightmapShape();
            }

            // Noise frequency
            if (ImGui::SliderFloat("Frequency", &config_.frequency, 0.2, 1.0,"%.3f"))
            {
                updateHeightmapShape();
            }

            // terrain width
            if (ImGui::SliderFloat("xSize", &config_.xSize, 2, 10,"%.1f"))
            {
                updateHeightmapShape();
            }
            // terrain depth
            if (ImGui::SliderFloat("ySize", &config_.ySize, 2, 10,"%.1f"))
            {
                updateHeightmapShape();
            }
            // Terrain width resolution
            if (ImGui::SliderFloat("Resolution ", &config_.resolution, 0.001, 1.0, "%.3f"))
            {
                updateHeightmapShape();
            }
            // Terrain width resolution
            if (ImGui::SliderInt( "Octaves ", &config_.numOctaves, 1, 10))
            {
                updateHeightmapShape();
            }
        }

    }

    template <typename S>
    dynamics::ShapePtr createHeightmapShape(TerrainGenerator& generator,
                                            TerrainConfig& config)
    {
        using Vector3 = Eigen::Matrix<S, 3, 1>;

        Terrain terrain = generator.generate(config);

        auto scale = Vector3(config.resolution,config.resolution, 1);

        auto terrainShape = std::make_shared<dynamics::HeightmapShape<S>>();
        terrainShape->setScale(scale);
        terrainShape->setHeightField(int(config.xSize / config.resolution) + 1,
                                     int(config.ySize / config.resolution) + 1,
                                     terrain.heights);

        return terrainShape;
    }


    void updateHeightmapShape()
    {
        mTerrain->setShape(createHeightmapShape<float>(generator_, config_));

        auto tf = mTerrain->getRelativeTransform();
        tf.translation()[0] = -static_cast<double>(config_.xSize) / 2.0;
        tf.translation()[1] = +static_cast<double>(config_.ySize) / 2.0;
        mTerrain->setRelativeTransform(tf);
    }

protected:

    TerrainConfig& config_;
    TerrainGenerator& generator_;
    dynamics::SimpleFramePtr mTerrain;
    dart::gui::osg::ImGuiViewer* mViewer;
};

#endif //TERRAINGENERATOR_HEIGHTMAPWIDGET_H
