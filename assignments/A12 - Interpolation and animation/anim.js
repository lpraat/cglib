function anim(cx, cy, cz, qx, qy, qz, qw, alpha) {
	// cx, cy, cz are arrays of four points
	// qx, qy, qz, qw are arrays of four quaternions
	// returns transform matrix with rotation and translation given
	// by Bezier interpolation of the input positions
  // according to parameter alpha (0 <= alpha <= 1)
	let q0 = new Quaternion(qw[0], qx[0], qy[0], qz[0]);
	let q1 = new Quaternion(qw[1], qx[1], qy[1], qz[1]);
	let q2 = new Quaternion(qw[2], qx[2], qy[2], qz[2]);
	let q3 = new Quaternion(qw[3], qx[3], qy[3], qz[3]);

  // Rotation
  const rotMatrix = qBerp(q0, q1, q2, q3, alpha).toMatrix4();

  // Translation
	const translateMatrix =  utils.MakeTranslateMatrix(berp(cx, alpha), berp(cy, alpha), berp(cz, alpha));

  return utils.multiplyMatrices(translateMatrix,rotMatrix);
}

function qBerp(q0, q1, q2, q3, alpha) {
  let q01 = q0.slerp(q1)(alpha);
  let q12 = q1.slerp(q2)(alpha);
  let q23 = q2.slerp(q3)(alpha);
  let q012 = q01.slerp(q12)(alpha);
  let q123 = q12.slerp(q23)(alpha);
  return q012.slerp(q123)(alpha);
}

function berp(v, alpha) {
  return Math.pow(1-alpha, 3)*v[0] + 3*Math.pow(1-alpha, 2)*alpha*v[1] + 3*(1-alpha)*alpha*alpha*v[2] + alpha*alpha*alpha*v[3];
}

