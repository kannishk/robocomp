#ifndef INNER_EIGEN_API
#define INNER_EIGEN_API

#include <QObject>
#include "../core/topics/IDLGraphPubSubTypes.h"
#include <Eigen/Geometry>

namespace DSR
{
    namespace Mat
    {
        using RTMat = Eigen::Transform<double, 3, Eigen::Affine>;
        using Rot3D = Eigen::Matrix3d;
        using Vector6d = Eigen::Matrix<double, 6, 1>;
    };

    class DSRGraph;

    class InnerEigenAPI : public QObject
    {
        Q_OBJECT
        using KeyTransform = std::tuple<std::string, std::string>;
        using NodeReference = std::map<int32_t, std::list<KeyTransform>>;
        using TransformCache = std::map<KeyTransform, Mat::RTMat>;
        using NodeMatrix = std::tuple<int32_t, Mat::RTMat>;
        using Lists = std::tuple<std::list<NodeMatrix>, std::list<NodeMatrix>>;
        public:
            explicit InnerEigenAPI(DSRGraph *G_);

            /////////////////////////////////////////////////
            /// Kinematic transformation methods
            ////////////////////////////////////////////////
            std::optional<Eigen::Vector3d> transform( const std::string &dest, const std::string &orig);
            std::optional<Eigen::Vector3d> transform( const std::string &dest, const Eigen::Vector3d &vector, const std::string &orig);
            std::optional<Mat::Vector6d> transform_axis(const std::string &dest, const std::string & orig);
            std::optional<Mat::Vector6d> transform_axis(const std::string &dest, const Mat::Vector6d &vector, const std::string &orig);

            ////////////////////////////////////////////////
            /// Transformation matrix retrieval methods
            ////////////////////////////////////////////////
            std::optional<Mat::RTMat> get_transformation_matrix(const std::string &dest, const std::string &orig);
            std::optional<Mat::RTMat> get_rotation_matrix(const std::string &dest, const std::string &orig){ return{};};
            std::optional<Mat::RTMat> get_translation_vector(const std::string &dest, const std::string &orig){return{};};
            std::optional<Mat::RTMat> get_euler_xyz_angles(const std::string &dest, const std::string &orig){return{};};

        public slots:
            void add_or_assign_edge_slot(const std::int32_t from, const std::int32_t to, const std::string& edge_type);
            void del_node_slot(const std::int32_t id);
            void del_edge_slot(const std::int32_t from, const std::int32_t to, const std::string &edge_type);

        private:
            DSR::DSRGraph *G;
            TransformCache cache;
            NodeReference node_map;
            std::optional<InnerEigenAPI::Lists> setLists(const std::string &orig, const std::string &dest);
            void remove_cache_entry(const std::int32_t id);
    };
}

#endif