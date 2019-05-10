#include <iostream>
#include "transform.h"
#include <vector>
#include "core_types.h"
#include "quat.h"
#include <cmath>
#include "constants.h"

using namespace glp;

int main() {
    Vec3 v {
        static_cast<float32>(std::cos(30 * pi() / 180)),
        static_cast<float32>(std::sin(30 * pi() / 180)),
        0.0f
        };

    Quat q(90.0f, v);
    q.print();

    Vec4 v1 {1.0f, 2.0f, 3.0f, 1.0f};

    q.toRotMatrix().print(); std::cout << std::endl;

    Vec4 m {q.toRotMatrix().dot(v1)};

    std::cout << "Resulting vector using quat" << std::endl;
    m.print(); std::cout << std::endl;

    Mat4 ra = q.toRotMatrix();
    auto res2 = extractEulerAngles(ra);

    std::vector<Mat4<float32>> composed2 = {
        rotateZ(res2.z * 180 / pi()),
        rotateY(res2.y * 180 / pi()),
        rotateX(res2.x * 180 / pi()),
    };

    Mat4 rotMatrix {compose(composed2)};
    rotMatrix.print(); std::cout << std::endl;

    Vec4 m2 {rotMatrix.dot(v1)};
     std::cout << "Resulting vector using euler" << std::endl;
     m2.print();
}