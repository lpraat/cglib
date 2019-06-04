
class Quat {
  constructor(a, b, c, d) {
    this.a = a;
    this.b = b;
    this.c = c;
    this.d = d;
  }

  static fromAngleAxis(theta, normalizedAxis) {
    const cosThetaHalved = Math.cos((theta * pi / 180 / 2));
    const sinThetaHalved = Math.sin((theta * pi / 180 / 2));

    return new Quat(cosThetaHalved, sinThetaHalved * normalizedAxis[0], sinThetaHalved * normalizedAxis[1], sinThetaHalved * normalizedAxis[2]);
  }

  mul(other) {
    let a = this.a * other.a - this.b * other.b - this.c * other.c - this.d * other.d;
    let b = this.a * other.b + this.b * other.a + this.c * other.d - this.d * other.c;
    let c = this.a * other.c + this.c * other.a + this.d * other.b - this.b * other.d;
    let d = this.a * other.d + this.d * other.a + this.b * other.c - this.c * other.b;
    return new Quat(a, b, c, d);
  }

  toRot(q) {
    return [1 - 2*(this.c*this.c + this.d*this.d), 2*(this.b*this.c - this.a*this.d), 2*(this.b*this.d +this.a*this.c), 0,
    2*(this.b*this.c + this.a*this.d), 1 - 2*(this.b*this.b + this.d*this.d), 2*(this.c*this.d - this.a*this.b), 0,
    2*(this.b*this.d - this.a*this.c), 2*(this.c*this.d + this.a*this.b), 1 - 2*(this.b*this.b + this.c*this.c), 0,
    0, 0, 0, 1];
  }

  length() {
    return Math.sqrt(this.a*this.a + this.b*this.b + this.c*this.c + this.d*this.d);
  }

  normalized() {
    const l = this.length();
    return new Quat(this.a /= l, this.b /= l, this.c /= l, this.d /= l);
  }
  conjugate() {
    return new Quat(this.a, -this.b, -this.c, -this.d);
  }

}

const pi = Math.atan(1)*4;

let orientation = Quat.fromAngleAxis(0, [0, 0, 0]); // Quat

// this function returns the world matrix with the updated rotations.
// parameters rvx, rvy and rvz contains a value in the -1 .. +1 range that tells the angular velocity of the world.
function updateWorld(rvx, rvy, rvz) {
  const qRoll = Quat.fromAngleAxis(rvz, [0, 0, 1]);
  const qPitch = Quat.fromAngleAxis(rvx, [1, 0, 0]);
  const qYaw = Quat.fromAngleAxis(rvy, [0, 1, 0]);

  let change = (qYaw.mul(qPitch).mul(qRoll));

  // change * orientation <-- we are rotating around the world axis
  // if instead orientation * change <-- we are rotating around the local axis, so we need to conjugate before getting the world matrix
  // so as to have (change * orientation behavior)
  orientation = change.mul(orientation).normalized();

  return orientation.toRot();
}

