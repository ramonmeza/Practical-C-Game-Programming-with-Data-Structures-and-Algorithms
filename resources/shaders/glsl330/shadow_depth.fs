#version 330

void main() {
    // Output depth as the fragment's depth
    gl_FragDepth = gl_FragCoord.z;
}
