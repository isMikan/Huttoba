xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 28;
 -0.71392;0.09981;-0.13784;,
 -0.58817;0.09981;0.00408;,
 -0.58817;0.05641;0.00408;,
 -0.71392;0.05641;-0.13784;,
 -0.58817;0.09981;0.00408;,
 -0.70873;0.09981;0.12083;,
 -0.70873;0.05641;0.12083;,
 -0.58817;0.05641;0.00408;,
 -0.70873;0.09981;0.12083;,
 -0.71392;0.09981;-0.13784;,
 -0.71392;0.05641;-0.13784;,
 -0.70873;0.05641;0.12083;,
 -0.58817;0.09981;0.00408;,
 -0.71392;0.09981;-0.13784;,
 -0.71392;0.05641;-0.13784;,
 -0.58817;0.05641;0.00408;,
 0.00424;0.09981;-0.13779;,
 0.00424;0.05641;-0.13779;,
 0.00424;0.09981;-0.13779;,
 0.00424;0.09981;0.11337;,
 0.00424;0.05641;0.11337;,
 0.00424;0.05641;-0.13779;,
 0.00424;0.09981;0.11337;,
 0.00424;0.05641;0.11337;,
 0.00424;0.09981;-0.13779;,
 -0.58817;0.09981;0.00408;,
 -0.58817;0.05641;0.00408;,
 0.00424;0.05641;-0.13779;;
 
 11;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 3;5,12,13;,
 3;14,15,6;,
 4;4,16,17,7;,
 4;18,19,20,21;,
 4;22,5,6,23;,
 4;8,1,2,11;,
 4;8,19,24,25;,
 4;26,27,20,11;;
 
 MeshMaterialList {
  1;
  11;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.746667;0.800000;0.611765;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  11;
  0.748458;0.000000;-0.663182;,
  0.695647;0.000000;0.718384;,
  0.380088;0.000000;0.924950;,
  -0.924637;0.000000;-0.380850;,
  -0.999799;0.000000;0.020044;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;0.000000;,
  -0.481342;0.000000;-0.876533;,
  -0.232891;0.000000;-0.972503;,
  1.000000;0.000000;0.000000;,
  0.010464;0.000000;0.999945;;
  11;
  4;0,0,0,0;,
  4;1,2,2,1;,
  4;3,4,4,3;,
  3;5,5,5;,
  3;6,6,6;,
  4;7,8,8,7;,
  4;9,9,9,9;,
  4;10,2,2,10;,
  4;3,7,7,3;,
  4;5,5,5,5;,
  4;6,6,6,6;;
 }
 MeshTextureCoords {
  28;
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;;
 }
}
