// Modified version of SpatialHash.hpp from IPC codebase.
// Originally created by Minchen Li.
#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <Eigen/Core>
#include <ipc/utils/eigen_ext.hpp>

#include <ipc/spatial_hash/collision_candidate.hpp>

namespace ipc {

class SpatialHash {
public: // data
    Eigen::VectorX3d leftBottomCorner, rightTopCorner;
    double one_div_voxelSize;
    Eigen::ArrayMax3i voxelCount;
    int voxelCount0x1;

    int edgeStartInd, triStartInd;

    std::unordered_map<int, std::vector<int>> voxel;
    std::vector<std::vector<int>> pointAndEdgeOccupancy;

    int dim;

public: // constructor
    SpatialHash() {}

    SpatialHash(
        const Eigen::MatrixXd& V,
        const Eigen::MatrixXi& E,
        const Eigen::MatrixXi& F,
        double voxelSize = -1)
    {
        build(V, E, F, voxelSize);
    }

    SpatialHash(
        const Eigen::MatrixXd& V0,
        const Eigen::MatrixXd& V1,
        const Eigen::MatrixXi& E,
        const Eigen::MatrixXi& F,
        double voxelSize = -1)
    {
        build(V0, V1, E, F, voxelSize);
    }

public: // API
    void build(
        const Eigen::MatrixXd& V,
        const Eigen::MatrixXi& E,
        const Eigen::MatrixXi& F,
        double voxelSize = -1);

    void build(
        const Eigen::MatrixXd& V0,
        const Eigen::MatrixXd& V1,
        const Eigen::MatrixXi& E,
        const Eigen::MatrixXi& F,
        double voxelSize = -1);

    void clear()
    {
        voxel.clear();
        pointAndEdgeOccupancy.clear();
    }

    void queryPointForTriangles(
        const Eigen::VectorX3d& p,
        std::unordered_set<int>& triInds,
        double radius = 0) const;

    void queryPointForTriangles(
        const Eigen::VectorX3d& p_t0,
        const Eigen::VectorX3d& p_t1,
        std::unordered_set<int>& triInds,
        double radius = 0) const;

    void queryPointForPrimitives(
        const Eigen::VectorX3d& p_t0,
        const Eigen::VectorX3d& p_t1,
        std::unordered_set<int>& vertInds,
        std::unordered_set<int>& edgeInds,
        std::unordered_set<int>& triInds) const;

    void queryEdgeForPE(
        const Eigen::VectorX3d& e0,
        const Eigen::VectorX3d& e1,
        std::vector<int>& vertInds,
        std::vector<int>& edgeInds) const;

    void queryEdgeForEdges(
        const Eigen::VectorX3d& ea0,
        const Eigen::VectorX3d& ea1,
        std::vector<int>& edgeInds,
        double radius = 0,
        int eai = -1) const;

    void queryEdgeForEdgesWithBBoxCheck(
        const Eigen::MatrixXd& V,
        const Eigen::MatrixXi& E,
        const Eigen::MatrixXi& F,
        const Eigen::VectorX3d& ea0,
        const Eigen::VectorX3d& ea1,
        std::vector<int>& edgeInds,
        double radius = 0,
        int eai = -1) const;

    void queryEdgeForEdges(
        const Eigen::VectorX3d& ea0_t0,
        const Eigen::VectorX3d& ea1_t0,
        const Eigen::VectorX3d& ea0_t1,
        const Eigen::VectorX3d& ea1_t1,
        std::vector<int>& edgeInds,
        double radius = 0,
        int eai = -1) const;

    void queryTriangleForPoints(
        const Eigen::VectorX3d& t0,
        const Eigen::VectorX3d& t1,
        const Eigen::VectorX3d& t2,
        std::unordered_set<int>& pointInds,
        double radius = 0) const;

    void queryTriangleForPoints(
        const Eigen::VectorX3d& t0_t0,
        const Eigen::VectorX3d& t1_t0,
        const Eigen::VectorX3d& t2_t0,
        const Eigen::VectorX3d& t0_t1,
        const Eigen::VectorX3d& t1_t1,
        const Eigen::VectorX3d& t2_t1,
        std::unordered_set<int>& pointInds) const;

    void queryTriangleForEdges(
        const Eigen::VectorX3d& t0,
        const Eigen::VectorX3d& t1,
        const Eigen::VectorX3d& t2,
        std::unordered_set<int>& edgeInds,
        double radius = 0) const;

    void queryEdgeForTriangles(
        const Eigen::VectorX3d& e0,
        const Eigen::VectorX3d& e1,
        std::unordered_set<int>& triInds,
        double radius = 0) const;

    void queryPointForPrimitives(
        int vi,
        std::unordered_set<int>& vertInds,
        std::unordered_set<int>& edgeInds,
        std::unordered_set<int>& triInds) const;

    void queryPointForEdges(int vi, std::unordered_set<int>& edgeInds) const;

    void queryPointForTriangles(int vi, std::unordered_set<int>& triInds) const;

    // will only put edges with larger than ei index into edgeInds
    void queryEdgeForEdges(int eai, std::unordered_set<int>& edgeInds) const;

    void queryEdgeForEdgesWithBBoxCheck(
        const Eigen::MatrixXd& V0,
        const Eigen::MatrixXd& V1,
        const Eigen::MatrixXi& E,
        const Eigen::MatrixXi& F,
        int eai,
        std::unordered_set<int>& edgeInds) const;

    void queryMeshForCandidates(
        const Eigen::MatrixXd& V,
        const Eigen::MatrixXi& E,
        const Eigen::MatrixXi& F,
        Candidates& candidates,
        double radius = 0,
        bool queryEV = false,
        bool queryEE = true,
        bool queryFV = true) const;

    void queryMeshForCandidates(
        const Eigen::MatrixXd& V0,
        const Eigen::MatrixXd& V1,
        const Eigen::MatrixXi& E,
        const Eigen::MatrixXi& F,
        Candidates& candidates,
        double radius = 0,
        bool queryEV = false,
        bool queryEE = true,
        bool queryFV = true) const;

public: // helper functions
    int locateVoxelIndex(const Eigen::VectorX3d& p) const;

    void locateVoxelAxisIndex(
        const Eigen::VectorX3d& p, Eigen::ArrayMax3i& voxelAxisIndex) const;

    int voxelAxisIndex2VoxelIndex(const int voxelAxisIndex[3]) const;

    int voxelAxisIndex2VoxelIndex(int ix, int iy, int iz) const;
};

} // namespace ipc
