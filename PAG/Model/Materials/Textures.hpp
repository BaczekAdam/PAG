// Textures.hpp
//
// Copyright (c) 2017 Krystian Owoc
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef Textures_hpp
#define Textures_hpp

#include <stdio.h>
#include <vector>
#include <assimp/scene.h>

class Texture;
class Shader;
struct Material;

class Textures
{
private:
    std::vector<Texture> mDiffuseTextures;
    std::vector<Texture> mSpecularTextures;
    std::vector<Texture> mNormalTextures;
    
    std::string mTexturesPath;
    
    void loadTextures(const aiScene* const pScene);
    const bool chcekIfIsLoaded(const std::string& pTexturePath, const std::string& pTextureType);
public:
    Textures(const aiScene* const pScene, const std::string& pTexturesPath, Shader *const pShader);
    const Material findTexturesForMaterial(aiMaterial* const pMaterial);
    void setActiveTextures(const Material& pMaterial, Shader *const pShader);
};


#endif /* Textures_hpp */
