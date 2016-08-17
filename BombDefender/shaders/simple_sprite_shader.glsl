precision highp float;

uniform mat4 proj_mv_matrix;
uniform sampler2D s_texture;
uniform float alpha;

#ifdef VERTEX

attribute vec2 a_texCoord;
attribute vec3 a_position;
varying vec2 v_texCoord;

void main() {
  gl_Position = proj_mv_matrix * vec4(a_position, 1.0);
  v_texCoord = a_texCoord;
}

#endif

#ifdef FRAGMENT

varying vec2 v_texCoord;
// out vec4 o_fragColor;

void main() {
//  gl_FragColor = alpha * texture2D( s_texture, v_texCoord );
  gl_FragColor = vec4(sin(10.0 * v_texCoord.x), 20.0 * v_texCoord.y, 50.0, 1.0);
}

#endif