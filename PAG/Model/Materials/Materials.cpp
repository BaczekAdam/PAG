// Materials.cpp
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

#include "Materials.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Config.hpp"

Materials::Materials(const aiScene* const pScene, const std::string& pTexturesPath, Shader *const pShader): mTexturesPath(pTexturesPath)
{
    loadTextures(pScene);
    
    int textureNumber[10], i;
    
    //Przypisywanie tekstur
    //Docelowo obsługa wielu tekstur
    for (i=0;i<10;i++)
        textureNumber[i]=i;
    pShader->setInt("diffuseTexture", textureNumber, 10);
    
    for (i=0;i<10;i++)
        textureNumber[i]=i+10;
    pShader->setInt("specularTexture", textureNumber, 10);
    
    for (i=0;i<10;i++)
        textureNumber[i]=i+20;
    pShader->setInt("normalTexture", textureNumber, 10);
}

void Materials::loadTextures(const aiScene* const pScene)
{
    int i,j;
    for (i=0;i<pScene->mNumMaterials;i++)
    {
        if (pScene->mMaterials[i]!=NULL)
        {
            //Diffuse
            for (j=0;j<pScene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE);j++)
            {
                aiString texture;
                std::string texturePath=mTexturesPath, textureName;
                pScene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, j, &texture);
                textureName=texture.C_Str();
                textureName=textureName.substr(textureName.rfind("/")+1);
                texturePath.append(textureName);
                if (!chcekIfIsLoaded(texturePath, DIFFUSE_NAME))
                {
                    try
                    {
                        mDiffuseTextures.push_back(Texture(texturePath));
                    } catch (std::runtime_error err) { throw err; }
                }
            }
            //Specular
            for (j=0;j<pScene->mMaterials[i]->GetTextureCount(aiTextureType_SPECULAR);j++)
            {
                aiString texture;
                std::string texturePath=mTexturesPath, textureName;
                pScene->mMaterials[i]->GetTexture(aiTextureType_SPECULAR, j, &texture);
                textureName=texture.C_Str();
                textureName=textureName.substr(textureName.rfind("/")+1);
                texturePath.append(textureName);
                if (!chcekIfIsLoaded(texturePath, SPECULAR_NAME))
                {
                    try
                    {
                        mSpecularTextures.push_back(Texture(texturePath));
                    } catch (std::runtime_error err) { throw err; }
                }
            }
            //Normal
            for (j=0;j<pScene->mMaterials[i]->GetTextureCount(aiTextureType_NORMALS);j++)
            {
                aiString texture;
                std::string texturePath=mTexturesPath, textureName;
                pScene->mMaterials[i]->GetTexture(aiTextureType_NORMALS, j, &texture);
                textureName=texture.C_Str();
                textureName=textureName.substr(textureName.rfind("/")+1);
                texturePath.append(textureName);
                if (!chcekIfIsLoaded(texturePath, NORMAL_NAME))
                {
                    try
                    {
                        mNormalTextures.push_back(Texture(texturePath));
                    } catch (std::runtime_error err) { throw err; }
                }
            }
            for (j=0;j<pScene->mMaterials[i]->GetTextureCount(aiTextureType_HEIGHT);j++) //Czasem normalne wykrywane przez assimpa są jako wysokości
            {
                aiString texture;
                std::string texturePath=mTexturesPath, textureName;
                pScene->mMaterials[i]->GetTexture(aiTextureType_HEIGHT, j, &texture);
                textureName=texture.C_Str();
                textureName=textureName.substr(textureName.rfind("/")+1);
                texturePath.append(textureName);
                if (!chcekIfIsLoaded(texturePath, NORMAL_NAME))
                {
                    try
                    {
                        mNormalTextures.push_back(Texture(texturePath));
                    } catch (std::runtime_error err) { throw err; }
                }
            }
            
            mMaterials.push_back(fillMaterialData(pScene->mMaterials[i]));
        }
    }
}

const bool Materials::chcekIfIsLoaded(const std::string& pTexturePath, const std::string& pTextureType)
{
    int i;
    if (pTextureType.compare(DIFFUSE_NAME)==0)
    {
        for (i=0;i<mDiffuseTextures.size();i++)
            if (mDiffuseTextures[i].getTexturePath().compare(pTexturePath)==0) return true;
    } else if (pTextureType.compare(SPECULAR_NAME)==0)
    {
        for (i=0;i<mSpecularTextures.size();i++)
            if (mSpecularTextures[i].getTexturePath().compare(pTexturePath)==0) return true;
    }
    else if (pTextureType.compare(NORMAL_NAME)==0)
    {
        for (i=0;i<mNormalTextures.size();i++)
            if (mNormalTextures[i].getTexturePath().compare(pTexturePath)==0) return true;
    }
    return false;
}

const Material Materials::fillMaterialData(aiMaterial* const pMaterial)
{
    Material output;
    aiColor3D temporaryColor;
    int i,j;
    //Diffuse
    pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, temporaryColor);
    output.mDiffuseColor=glm::vec4(temporaryColor.r, temporaryColor.g, temporaryColor.b, 1);
    for (i=0;i<pMaterial->GetTextureCount(aiTextureType_DIFFUSE);i++)
    {
        aiString texture;
        std::string texturePath=mTexturesPath, textureName;
        pMaterial->GetTexture(aiTextureType_DIFFUSE, i, &texture);
        textureName=texture.C_Str();
        textureName=textureName.substr(textureName.rfind("/")+1);
        texturePath.append(textureName);
        for (j=0;j<mDiffuseTextures.size();j++)
            if (mDiffuseTextures[j].getTexturePath().compare(texturePath)==0) output.mDiffuseTextureID.push_back(j);
    }
    //Specular
    pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, temporaryColor);
    output.mSpecularColor=glm::vec4(temporaryColor.r, temporaryColor.g, temporaryColor.b, 1);
    for (i=0;i<pMaterial->GetTextureCount(aiTextureType_SPECULAR);i++)
    {
        aiString texture;
        std::string texturePath=mTexturesPath, textureName;
        pMaterial->GetTexture(aiTextureType_SPECULAR, i, &texture);
        textureName=texture.C_Str();
        textureName=textureName.substr(textureName.rfind("/")+1);
        texturePath.append(textureName);
        for (j=0;j<mSpecularTextures.size();j++)
            if (mSpecularTextures[j].getTexturePath().compare(texturePath)==0) output.mSpecularTextureID.push_back(j);
    }
    //Ambient
    pMaterial->Get(AI_MATKEY_COLOR_AMBIENT, temporaryColor);
    output.mAmbientColor=glm::vec4(temporaryColor.r, temporaryColor.g, temporaryColor.b, 1);
    //Normal
    for (i=0;i<pMaterial->GetTextureCount(aiTextureType_NORMALS);i++)
    {
        aiString texture;
        std::string texturePath=mTexturesPath, textureName;
        pMaterial->GetTexture(aiTextureType_NORMALS, i, &texture);
        textureName=texture.C_Str();
        textureName=textureName.substr(textureName.rfind("/")+1);
        texturePath.append(textureName);
        for (j=0;j<mNormalTextures.size();j++)
            if (mNormalTextures[j].getTexturePath().compare(texturePath)==0) output.mNomralTextureID.push_back(j);
    }
    for (i=0;i<pMaterial->GetTextureCount(aiTextureType_HEIGHT);i++) //Czasem normalne wykrywane przez assimpa są jako wysokości
    {
        aiString texture;
        std::string texturePath=mTexturesPath, textureName;
        pMaterial->GetTexture(aiTextureType_HEIGHT, i, &texture);
        textureName=texture.C_Str();
        textureName=textureName.substr(textureName.rfind("/")+1);
        texturePath.append(textureName);
        for (j=0;j<mNormalTextures.size();j++)
            if (mNormalTextures[j].getTexturePath().compare(texturePath)==0) output.mNomralTextureID.push_back(j);
    }
    
    //Shininess
    pMaterial->Get(AI_MATKEY_SHININESS, output.mShininess); //Assimp nie normalizuje tych wartości, każdy format może mieć inne
    pMaterial->Get(AI_MATKEY_SHININESS_STRENGTH, output.mShininessStrength); //Np fbx ma tu taką samą wartość co w shininess, obj ma zazwyczaj 1
    
    //Shading mode
    if (pMaterial->Get(AI_MATKEY_SHADING_MODEL, output.mShadingMode)!=AI_SUCCESS)
        output.mShadingMode=DEFAULT_SHADING_MODE; //Domyślne cieniowanie w przypadku niepowodzenia
    
    return output;
}

void Materials::setActiveMaterial(const unsigned int& pMaterialID, Shader *const pShader)
{
    int i;
    
    pShader->setInt("shadingMode", mMaterials[pMaterialID].mShadingMode);
    
    pShader->setVec3("diffuseColor", &mMaterials[pMaterialID].mDiffuseColor);
    pShader->setVec3("specularColor", &mMaterials[pMaterialID].mSpecularColor);
    pShader->setVec3("ambientColor", &mMaterials[pMaterialID].mAmbientColor);
    pShader->setFloat("shininess", &mMaterials[pMaterialID].mShininess);
    pShader->setFloat("shininessStrength", &mMaterials[pMaterialID].mShininessStrength);
    //Sprawdzanie czy materiał używa tekstur
    if (mMaterials[pMaterialID].mDiffuseTextureID.size()==0) pShader->setBool("shouldUseDiffuseTexture", false);
    else pShader->setBool("shouldUseDiffuseTexture", true);
    if (mMaterials[pMaterialID].mSpecularTextureID.size()==0)
    {
        pShader->setBool("shouldUseSpecularTexture", false);
    }
    else pShader->setBool("shouldUseSpecularTexture", true);
    if (mMaterials[pMaterialID].mNomralTextureID.size()==0)
    {
        pShader->setBool("shouldUseNormalTexture", false);
    }
    else pShader->setBool("shouldUseNormalTexture", true);
    
    //Przypisywanie tekstur
    Texture::deselectAllTextures();
    for (i=0;i<mMaterials[pMaterialID].mDiffuseTextureID.size();i++)
        mDiffuseTextures[mMaterials[pMaterialID].mDiffuseTextureID[i]].selectActiveTexture(DIFFUSE_NAME, i);
    for (i=0;i<mMaterials[pMaterialID].mSpecularTextureID.size();i++)
        mSpecularTextures[mMaterials[pMaterialID].mSpecularTextureID[i]].selectActiveTexture(SPECULAR_NAME, i);
    for (i=0;i<mMaterials[pMaterialID].mNomralTextureID.size();i++)
        mNormalTextures[mMaterials[pMaterialID].mNomralTextureID[i]].selectActiveTexture(NORMAL_NAME, i);
}

void Materials::setDefaultMaterial(Shader *const pShader)
{
    Material temporaryMaterial;
    
    pShader->setInt("shadingMode", temporaryMaterial.mShadingMode);
    pShader->setVec3("diffuseColor", &temporaryMaterial.mDiffuseColor);
    pShader->setBool("shouldUseDiffuseTexture", false);
    
    Texture::deselectAllTextures();
}

