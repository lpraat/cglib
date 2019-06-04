function buildGeometry() {
	var i;

  // Draws a cube
  var vert1 = [[0, 1, -0.5],
               [-0.5, 0, -1],
               [-0.5, 0, 0],
               [0.5, 0, 0],
               [0.5, 0, -1]];
  var norm1 = [[0, 1, 0],
               []];
  var color1 = [0.0, 0.0, 1.0];

  var ind1 = [0, 1, 2,  0, 2, 3,  0, 3, 4,  0, 4, 1,  3, 2, 1,  3, 1, 4];

	addMesh(vert1, norm1, ind1, color1);

	// Draws a Cylinder
	var vert2 = [[0.0, 0.0, 0.0]];
	var norm2 = [[0.0, 0.0, 1.0]];
	var ind2 = [];
	var color2 = [1.0, 0.0, 1.0];
	var slices2 = 8;
	for(i = 0; i < slices2; i++) {
		vert2[i+1] = [Math.cos(2*Math.PI / slices2 * i), Math.sin(2*Math.PI / slices2 * i), 0.0];
		norm2[i+1] = [Math.cos(2*Math.PI / slices2 * i), Math.sin(2*Math.PI / slices2 * i), 1.0];
		ind2[3*i]   = 0;
		ind2[3*i+1] = i+1;
		ind2[3*i+2] = (i < slices2-1) ? i+2 : 1 ;
	}
	addMesh(vert2, norm2, ind2, color2);


	// Draws a Cone
	var vert3 = [[0.0, 0.0, 0.0]];
	var norm3 = [[0.0, 0.0, 1.0]];
	var ind3 = [];
	var color3 = [1.0, 1.0, 0.0];
	var slices3 = 64;
	for(i = 0; i < slices3; i++) {
		vert3[i+1] = [Math.cos(2*Math.PI / slices3 * i), Math.sin(2*Math.PI / slices3 * i), 0.0];
		norm3[i+1] = [Math.cos(2*Math.PI / slices3 * i), Math.sin(2*Math.PI / slices3 * i), 1.0];
		ind3[3*i]   = 0;
		ind3[3*i+1] = i+1;
		ind3[3*i+2] = (i < slices3-1) ? i+2 : 1 ;
	}
	addMesh(vert3, norm3, ind3, color3);

	// Draws a Sphere
	var vert4 = [[0.0, 0.0, 0.0]];
	var norm4 = [[0.0, 0.0, 1.0]];
	var ind4 = [];
	var color4 = [0.0, 1.0, 1.0];
	var slices4 = 5;
	for(i = 0; i < slices4; i++) {
		vert4[i+1] = [Math.sin(2*Math.PI / slices4 * i), -Math.cos(2*Math.PI / slices4 * i), 0.0];
		vert4[i+slices4+1] = [2.6*Math.sin(2*Math.PI / slices4 * (i+0.5)), -2.6*Math.cos(2*Math.PI / slices4 * (i+0.5)), 0.0];
		norm4[i+1] = [0.0, 0.0, 1.0];
		norm4[i+slices4+1] = [0.0, 0.0, 1.0];
		ind4[6*i]   = 0;
		ind4[6*i+1] = i+1;
		ind4[6*i+2] = (i < slices4-1) ? i+2 : 1 ;
		ind4[6*i+4] = i+1;
		ind4[6*i+3] = (i < slices4-1) ? i+2 : 1 ;
		ind4[6*i+5] = slices4 + i + 1;
	}
	addMesh(vert4, norm4, ind4, color4);
}

