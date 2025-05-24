#include <ArduinoEigenDense.h>
#include <command_line_interface.h>

#include <cmath>
#include <system_boot.hpp>

Command_Line_Interface CLI;

/**
 * 构造齐次旋转-平移变换矩阵
 * @param u     任意非零旋转轴向量
 * @param theta 旋转角度（弧度）
 * @param p     平移向量
 * @return      4×4 齐次变换矩阵
 */
Eigen::Matrix4d rodriguesTransform(const Eigen::Vector3d& u, double theta, const Eigen::Vector3d& p) {
    Eigen::Matrix4d T = Eigen::Matrix4d::Identity();  // 初始化为单位矩阵
    // 如果旋转角接近零，则仅执行平移
    if (std::abs(theta) < std::numeric_limits<double>::epsilon() || std::abs(theta) < std::numeric_limits<double>::min()) {
        T.block<3, 1>(0, 3) = p;  // 拼装平移块
        return T;                 // 返回齐次平移矩阵
    }
    Eigen::Vector3d axis = u.normalized();                                  // 单位化旋转轴
    Eigen::Matrix3d R = Eigen::AngleAxisd(theta, axis).toRotationMatrix();  // 构造旋转矩阵
    // 拼装齐次变换矩阵
    T.block<3, 3>(0, 0) = R;  // 拼装旋转块
    T.block<3, 1>(0, 3) = p;  // 拼装平移块
    return T;
}

/**
 * @brief 从齐次旋转–平移矩阵中反解旋转轴、旋转角和平移向量
 * @param[in]  T       输入的 4×4 齐次变换矩阵
 * @param[out] u_out   单位旋转轴向量
 * @param[out] theta   旋转角度，单位为弧度
 * @param[out] p_out   平移向量
 */
void inverseRodriguesTransform(const Eigen::Matrix4d& T, Eigen::Vector3d& u_out, double& theta, Eigen::Vector3d& p_out) {
    p_out = T.block<3, 1>(0, 3);              // 1) 提取平移
    Eigen::Matrix3d R = T.block<3, 3>(0, 0);  // 2) 提取旋转子矩阵
    Eigen::AngleAxisd aa(R);                  // 3) 用 Eigen 内置的 AngleAxisd 提取旋转轴和旋转角

    // 4) 处理数值边界：确保 angle ∈ [0, π]
    theta = aa.angle();
    if (theta < 0) {
        theta = -theta;
        aa.axis() = -aa.axis();
    }

    u_out = aa.axis().normalized();  // 5) 单位化旋转轴
}
/*void inverseRodriguesTransform(const Eigen::Matrix4f& T, Eigen::Vector3f& u_out, float& theta, Eigen::Vector3f& p_out) {
    // 1. 提取平移向量 p
    p_out = T.block<3, 1>(0, 3);  ///< 右上 3×1 区块即为平移

    // 2. 提取旋转子矩阵 R
    Eigen::Matrix3f R = T.block<3, 3>(0, 0);  ///< 左上 3×3 区块为旋转矩阵

    // 3. 计算旋转角 theta ∈ [0, π]
    float trace = R.trace();                  ///< 计算迹
    float cos_theta = (trace - 1.0f) * 0.5f;  ///< (tr(R)-1)/2
    // 手动限幅防止数值超出 [-1,1]
    if (cos_theta > 1.0f) cos_theta = 1.0f;
    if (cos_theta < -1.0f) cos_theta = -1.0f;
    theta = std::acos(cos_theta);  ///< 得到角度

    // 4. 反解旋转轴 u
    float s = std::sin(theta);
    if (std::abs(s) > std::numeric_limits<float>::epsilon()) {
        // 常规情况：sin(theta) ≠ 0
        u_out.x() = R(2, 1) - R(1, 2);  ///< u_x = R32 - R23
        u_out.y() = R(0, 2) - R(2, 0);  ///< u_y = R13 - R31
        u_out.z() = R(1, 0) - R(0, 1);  ///< u_z = R21 - R12
        u_out /= (2.0f * s);            ///< 单位化因子 1/(2 sinθ)
    } else {
        // 特殊情况：θ ≈ 0 或 π，求解 (R - I) u = 0
        Eigen::Matrix3f M = R - Eigen::Matrix3f::Identity();
        Eigen::EigenSolver<Eigen::Matrix3f> es(M);
        // 从特征值中选取实部最接近 0 的一列对应的特征向量
        int idx = 0;
        for (int i = 0; i < 3; ++i) {
            if (std::abs(es.eigenvalues()[i].imag()) < std::numeric_limits<float>::epsilon()) {
                idx = i;
                break;
            }
        }
        u_out = es.eigenvectors().col(idx).real();  ///< 取实部
        u_out.normalize();                          ///< 单位化
    }
}*/

void setup() {
    Serial.begin(115200);
    system_boot();
    delay(4000);

    Eigen::Vector3d u_out;
    double theta;
    Eigen::Vector3d p_out;

    Eigen::Vector3d u(0.142, 0.576, -0.71);
    Eigen::Vector3d p(0, 0, 0);

    for (double i = 0; i <= 3 * M_PI; i += M_PI / 5) {
        Eigen::Matrix4d m = rodriguesTransform(u, i, p);
        inverseRodriguesTransform(m, u_out, theta, p_out);

        Serial.print("\r\n旋转矩阵:\r\n");
        std::cout << m << std::endl;
        Serial.print("\r\n旋转轴:\r\n");
        std::cout << u_out << std::endl;
        Serial.print("\r\n旋转角度:\r\n");
        std::cout << theta << std::endl;
        Serial.print("-----------------------------------------------");
    }

    // std::cout << m << std::endl;
    // std::cout << u_out << std::endl;
    // std::cout << theta << std::endl;
    //  std::cout << p_out << std::endl;
}

void loop() { CLI.run(); }
