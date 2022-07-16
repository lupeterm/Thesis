from typing import Dict, List, Set, Tuple
import open3d as o3d
import numpy as np
import sys
import os


class Plane:
    def __init__(self, points: np.array, index: int, name: str):
        self.inliers = points
        self.index = index
        self.name = name
        self.voxels = set()
        self.correspondence = -1  # -1: not found
        self.found = False


def read_planes(folder: str):
    planes: List[Plane] = []
    for filename in os.listdir(folder):
        if filename.endswith('.txt'):
            # only use x,y,z values (ignore rgb)
            points = np.loadtxt(f'{folder}/{filename}', usecols=(0, 1, 2))
            name = filename.replace('.txt', '')
            planes.append(Plane(points, len(planes), name))
    return planes


def calc_plane_voxels(planes: List[Plane], voxel_grid: o3d.geometry.VoxelGrid):
    plane_voxel: Dict[int, Set[Tuple[int, int, int]]] = dict()
    # calculate corresponding voxels for each plane
    for plane in planes:
        for point in plane.inliers:
            corr_voxel = tuple(voxel_grid.get_voxel(point))
            if plane.index not in plane_voxel.keys():
                plane_voxel[plane.index] = set()
            plane_voxel[plane.index].add(corr_voxel)
    return plane_voxel


if __name__ == '__main__':
    if len(sys.argv) < 4:
        print('Too few arguments!')
        print(
            'Usage: python evaluate.py [cloud].pcd [found_planes_folder] [gt_folder]')
        sys.exit(1)
    if len(sys.argv) > 3:
        pcd_file = sys.argv[1]
        planes_folder = sys.argv[2]
        gt_folder = sys.argv[3]
    # uncomment for
    pcd_file = 'Stanford3dDataset_v1.2_Aligned_Version/Area_1/copyRoom_1/Annotations_out/wall_3.txt_out.pcd'
    # planes_folder = 'EVAL/TESTGT'
    # gt_folder = 'EVAL/TESTGT'

    # read planes for evaluation
    ALGO_planes = read_planes(planes_folder)
    GT_planes = read_planes(gt_folder)

    # create Voxelization of complete Point cloud
    pcd = o3d.io.read_point_cloud(pcd_file)
    voxel_grid = o3d.geometry.VoxelGrid.create_from_point_cloud(
        pcd, voxel_size=0.05)

    # draw voxelgrid if needed
    o3d.visualization.draw_geometries(
        [voxel_grid])

    # calculate corresponding voxels for all planes
    GT_plane_voxel = calc_plane_voxels(GT_planes, voxel_grid)
    ALGO_plane_voxel = calc_plane_voxels(ALGO_planes, voxel_grid)

    for gt_plane_index, gtp_voxels in GT_plane_voxel.items():
        for algo_plane_index, ap_voxels in ALGO_plane_voxel.items():
            correspondence = len(gtp_voxels & ap_voxels) / len(gtp_voxels)
            # if more than 50% of voxels match
            if correspondence > 0.5:
                # add or update best match
                if correspondence > GT_planes[gt_plane_index].correspondence:
                    GT_planes[gt_plane_index].correspondence = correspondence
                    GT_planes[gt_plane_index].ap_index = algo_plane_index
                GT_planes[gt_plane_index].found = True

    print('found planes:')
    for p in GT_planes:
        if p.found:
            print(f'\t{p.name = } with {p.correspondence = }')
            best_match = ALGO_planes[p.ap_index].name
            print(f'\tbest match: {best_match}')

    # TODO:
    # Modify GT -> only plane inliers
    # implement saving of inlier points to file for algorithms
