#include <cmath>
#include <complex>
#include <vector>

const double PI = acos(-1);

// FFT递归函数
void FFT(std::vector<std::complex<double>>& a, bool inverse) {
    int n = a.size();
    if (n == 1) {
        return;
    }

    std::vector<std::complex<double>> even(n / 2), odd(n / 2);
    for (int i = 0; i < n / 2; ++i) {
        even[i] = a[2 * i];
        odd[i] = a[2 * i + 1];
    }

    FFT(even, inverse);
    FFT(odd, inverse);

    double angle = 2 * PI / n * (inverse ? -1 : 1);
    std::complex<double> w(1), wn(cos(angle), sin(angle));
    for (int i = 0; i < n / 2; ++i) {
        a[i] = even[i] + w * odd[i];
        a[i + n / 2] = even[i] - w * odd[i];
        if (inverse) {
            a[i] /= 2;
            a[i + n / 2] /= 2;
        }
        w *= wn;
    }
}

// 快速傅里叶变换
std::vector<std::complex<double>> fastFourierTransform(const std::vector<std::complex<double>>& input, bool inverse) {
    std::vector<std::complex<double>> a = input;
    FFT(a, inverse);
    return a;
}
