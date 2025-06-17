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
Eigen::Matrix4f rodriguesTransform(const Eigen::Vector3f& u, double theta, const Eigen::Vector3f& p) {
    Eigen::Matrix4f T = Eigen::Matrix4f::Identity();  // 初始化为单位矩阵
    // 如果旋转角接近零，则仅执行平移
    if (std::abs(theta) < std::numeric_limits<double>::epsilon() || std::abs(theta) < std::numeric_limits<double>::min()) {
        T.block<3, 1>(0, 3) = p;  // 拼装平移块
        return T;                 // 返回齐次平移矩阵
    }
    Eigen::Vector3f axis = u.normalized();                                  // 单位化旋转轴
    Eigen::Matrix3f R = Eigen::AngleAxisf(theta, axis).toRotationMatrix();  // 构造旋转矩阵
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
void inverseRodriguesTransform(const Eigen::Matrix4f& T, Eigen::Vector3f& u_out, double& theta, Eigen::Vector3f& p_out) {
    p_out = T.block<3, 1>(0, 3);              // 1) 提取平移
    Eigen::Matrix3f R = T.block<3, 3>(0, 0);  // 2) 提取旋转子矩阵
    Eigen::AngleAxisf aa(R);                  // 3) 用 Eigen 内置的 AngleAxisf 提取旋转轴和旋转角

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

/**
 * @brief 对三维向量使用四元数进行旋转
 * @details
 *   - 输入四元数无需预先单位化，仅当其模长平方偏离 1 超过阈值时才执行单位化，以兼顾性能与精度。
 *   - 使用 Eigen 重载运算符 q * v 自动执行 p' = q * [0, v] * q⁻¹ 的旋转逻辑。
 *
 * @param[in] v_in   原始三维向量（Eigen::Vector3f）
 * @param[in] q_in   表示旋转的四元数（Eigen::Quaternionf）
 * @return           旋转后的三维向量（Eigen::Vector3f）
 */
Eigen::Vector3f rotateTransform(const Eigen::Vector3f& v_in, const Eigen::Quaternionf& q_in) {
    // 计算四元数模长的平方，避免频繁调用 sqrtf
    const float norm2 = q_in.dot(q_in);

    // 如果模长平方与 1 相差较大，则做单位化处理
    Eigen::Quaternionf q(q_in);        ///< 本地副本，保护 q_in
    constexpr float NORM_EPS = 1e-6f;  ///< 单位化阈值
    if (fabsf(norm2 - 1.0f) > NORM_EPS) {
        // 计算逆模长并缩放系数
        const float invNorm = 1.0f / sqrtf(norm2);
        q.coeffs() *= invNorm;  ///< 单位化 x,y,z,w
    }

    // 调用 Eigen 的重载，执行四元数对向量的旋转
    return q * v_in;  ///< 返回旋转结果
}

/**
 * @brief 轴–角表示转换为单位四元数
 * @details
 *   - 输入轴向量可为任意非零向量，内部自动单位化。
 *   - 采用 q = [cos(θ/2), u * sin(θ/2)] 构造方式。
 *
 * @param[in] axis  任意旋转轴向量（Eigen::Vector3f）
 * @param[in] theta 旋转角度，单位：弧度（float）
 * @return          对应的单位四元数（Eigen::Quaternionf）
 */
Eigen::Quaternionf axisAngleToQuat(const Eigen::Vector3f& axis, float theta) {
    // 轴向量单位化，保证 ‖u‖ = 1
    const Eigen::Vector3f u = axis.normalized();  ///< u = axis / ‖axis‖

    // 计算半角的 sin 和 cos
    const float halfTheta = 0.5f * theta;  ///< θ/2
    const float sinH = sinf(halfTheta);    ///< sin(θ/2)
    const float cosH = cosf(halfTheta);    ///< cos(θ/2)

    // 构造并返回四元数，参数顺序为 (w, x, y, z)
    return Eigen::Quaternionf(cosH,          ///< w 分量
                              u.x() * sinH,  ///< x 分量
                              u.y() * sinH,  ///< y 分量
                              u.z() * sinH   ///< z 分量
    );
}

/**
 * @brief 四元数转换为轴–角表示
 * @details
 *   - 如输入四元数未单位化，会按需单位化。
 *   - θ = 2 * atan2(‖v‖, w)，axis = v/‖v‖；当 ‖v‖≈0 时返回 X 轴。
 *
 * @param[in]  q_in   任意四元数（Eigen::Quaternionf）
 * @param[out] axis   单位旋转轴（Eigen::Vector3f&）
 * @param[out] theta  旋转角，单位：弧度（float&）
 */
void quatToAxisAngle(const Eigen::Quaternionf& q_in, Eigen::Vector3f& axis, float& theta) {
    // 计算模长平方以判断是否需要单位化
    const float norm2 = q_in.dot(q_in);

    // 复制并按需单位化四元数
    Eigen::Quaternionf q(q_in);        ///< 保护原输入
    constexpr float NORM_EPS = 1e-6f;  ///< 单位化阈值
    if (fabsf(norm2 - 1.0f) > NORM_EPS) {
        const float invNorm = 1.0f / sqrtf(norm2);
        q.coeffs() *= invNorm;  ///< 缩放 x,y,z,w
    }

    // 拆分实部 w 与虚部 v
    const float w = q.w();                         ///< 四元数实部
    const Eigen::Vector3f v(q.x(), q.y(), q.z());  ///< 四元数虚部

    // 计算旋转角 θ = 2 * atan2(‖v‖, w)
    const float vnorm = v.norm();     ///< ‖v‖
    theta = 2.0f * atan2f(vnorm, w);  ///< 输出 θ

    // 虚部模长足够大时正常单位化，否则默认 X 轴
    if (vnorm > NORM_EPS) {
        axis = v / vnorm;  ///< 单位化虚部
    } else {
        axis = Eigen::Vector3f::UnitX();  ///< 零旋转选择 X 轴
    }
}

void setup() {
    Serial.begin(115200);
    system_boot();
    delay(4000);

    Eigen::Vector3f u_out;
    Eigen::Vector3f p_out;

    Eigen::Vector3f axis;
    float theta;

    Eigen::Vector3f u(0, 0, 1);
    Eigen::Vector3f p(1, 0, 0);

    for (double i = 0; i <= 3 * M_PI; i += M_PI / 6) {
        Eigen::Quaternionf q = axisAngleToQuat(u, i);
        Eigen::Vector3f v = rotateTransform(p, q);
        quatToAxisAngle(q, axis, theta);

        Serial.print("\r\n旋转轴:\r\n");
        std::cout << axis << std::endl;
        Serial.print("\r\n旋转角度:\r\n");
        std::cout << theta * (180 / M_PI) << std::endl;
        Serial.print("\r\n旋转结果:\r\n");
        std::cout << v << std::endl;
    }

    /*
        for (double i = 0; i <= 3 * M_PI; i += M_PI / 5) {
        Eigen::Matrix4f m = rodriguesTransform(u, i, p);
        inverseRodriguesTransform(m, u_out, theta, p_out);

        Serial.print("\r\n旋转矩阵:\r\n");
        std::cout << m << std::endl;
        Serial.print("\r\n旋转轴:\r\n");
        std::cout << u_out << std::endl;
        Serial.print("\r\n旋转角度:\r\n");
        std::cout << theta << std::endl;
        Serial.print("-----------------------------------------------");
    }
    */
}

void loop() { CLI.run(); }
