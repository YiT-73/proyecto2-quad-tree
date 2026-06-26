#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include "Structure.h"
#include <functional>
#include <mutex>
#include <string>
#include <vector>

void generateVisualization(double width, double height, double radius,
                           int capacity, const std::string &fileName,
                           int particleCount);

void generateVisualizationFrames(double width, double height, double radius,
                                 int capacity, const std::string &fileName,
                                 int particleCount, int totalFrames);

std::string generateVisualizationFramesJson(double width, double height,
                                            double radius, int capacity,
                                            int particleCount, int totalFrames,
                                            int distributionType);

void streamSimulationFrames(double width, double height, double radius,
                            int capacity, int particleCount,
                            int distributionType,
                            std::function<bool(const std::string &)> onFrame);

#endif

void streamSimulationLive(std::mutex &mtx,
                          std::vector<Particle> &sharedParticles, double width,
                          double height, double radius, int capacity,
                          std::function<bool(const std::string &)> onFrame);
