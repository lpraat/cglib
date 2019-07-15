function view(cx, cy, cz, alpha, beta, rho) {
	// Create a view matrix for a camera in position cx, cy and cz, looking in the direction specified by
  // alpha, beta and rho, as outlined in the course slides.

  let tMatrix = utils.MakeTranslateMatrix(-cx, -cy, -cz);
  let ry = utils.MakeRotateYMatrix(-alpha);
  let rx = utils.MakeRotateXMatrix(-beta);
  let rz = utils.MakeRotateZMatrix(-rho);




  // out = (translate * roty * rotx * rotz)^-1 = rotz^-1 * rotx^-1 * rot^-1 * translate^-1
  // out = rz * rx * ry * tMatrix
  var out = rz;
  out = utils.multiplyMatrices(out, rx);
  out = utils.multiplyMatrices(out, ry);
  out = utils.multiplyMatrices(out, tMatrix);

	return out;
}

