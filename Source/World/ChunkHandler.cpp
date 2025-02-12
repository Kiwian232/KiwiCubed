#include "ChunkHandler.h"

#include "World.h"
#include "glad/glad.h"
#include <mutex>


void ChunkHandler::GenerateWorld() {
    world.GenerateWorld();
}

Chunk& ChunkHandler::GetChunk(int chunkX, int chunkY, int chunkZ) {
    //std::lock_guard<std::recursive_mutex> lock(ChunkMutex);
    auto chunk = chunks.find(std::make_tuple(chunkX, chunkY, chunkZ));
    if (chunk != chunks.end()) {
        return chunk->second;
    }
    else {
        //std::cout << "[ChunkHandler / Debug] Chunk not found at {" << chunkX << ", " << chunkY << ", " << chunkZ << "}" << std::endl;
        Chunk& chunk = AddChunk(chunkX, chunkY, chunkZ);
        return chunk;
    }
}

bool ChunkHandler::GetChunkExists(int chunkX, int chunkY, int chunkZ) {
    auto chunk = chunks.find(std::make_tuple(chunkX, chunkY, chunkZ));
    if (chunk != chunks.end()) {
        return true;
    }
    else {
        return false;
    }
}

Chunk& ChunkHandler::AddChunk(int chunkX, int chunkY, int chunkZ) {
    auto chunk = chunks.find(std::make_tuple(chunkX, chunkY, chunkZ));
    if (chunk == chunks.end()) {

        chunks.insert(std::make_pair(std::tuple<int, int, int>(chunkX, chunkY, chunkZ), Chunk(chunkX, chunkY, chunkZ, renderer)));
        chunk = chunks.find(std::make_tuple(chunkX, chunkY, chunkZ));
        chunk->second.SetPosition(chunkX, chunkY, chunkZ);
        chunk->second.AllocateChunk();
        chunk->second.SetupRenderComponents();

        world.totalChunks++;

        return chunk->second;
    }
    else {
        //std::cout << "[ChunkHandler / Debug] Chunk already found at {" << chunkX << ", " << chunkY << ", " << chunkZ << "}" << std::endl;
        return GetChunk(chunkX, chunkY, chunkZ);
    }
}

void ChunkHandler::GenerateChunk(int chunkX, int chunkY, int chunkZ, Chunk& callerChunk, bool updateCallerChunk, bool debug) {
    //std::lock_guard<std::recursive_mutex> lock(ChunkMutex);
    Chunk& chunk = GetChunk(chunkX, chunkY, chunkZ);
    chunk.GenerateBlocks(world, callerChunk, updateCallerChunk, debug);
}

void ChunkHandler::MeshChunk(int chunkX, int chunkY, int chunkZ) {
    GetChunk(chunkX, chunkY, chunkZ).GenerateMesh(*this, false);
}

// Specifically uses the world's GenerateChunk() function that makes sure chunks mesh correctly
void ChunkHandler::SmartGenerateAndMeshChunk(int chunkX, int chunkY, int chunkZ) {
    Chunk chunk = GetChunk(chunkX, chunkY, chunkZ);
    world.GenerateChunk(chunkX, chunkY, chunkZ, chunk, false, chunk);
}

// Meshes a chunk fully no matter what, even if it needs to alloate / generate surrounding chunks too
void ChunkHandler::ForceGenerateAndMeshChunk(int chunkX, int chunkY, int chunkZ) {
    Chunk defaultChunk = Chunk(0, 0, 0, renderer);
    AddChunk(chunkX, chunkY, chunkZ);
    AddChunk(chunkX - 1, chunkY, chunkZ);
    AddChunk(chunkX, chunkY - 1, chunkZ);
    AddChunk(chunkX, chunkY, chunkZ - 1);
    AddChunk(chunkX + 1, chunkY, chunkZ);
    AddChunk(chunkX, chunkY + 1, chunkZ);
    AddChunk(chunkX, chunkY, chunkZ + 1);
    GenerateChunk(chunkX, chunkY, chunkZ, defaultChunk, false, true);
    GenerateChunk(chunkX - 1, chunkY, chunkZ, defaultChunk, false, true);
    GenerateChunk(chunkX, chunkY - 1, chunkZ, defaultChunk, false, true);
    GenerateChunk(chunkX, chunkY, chunkZ - 1, defaultChunk, false, true);
    GenerateChunk(chunkX + 1, chunkY, chunkZ, defaultChunk, false, true);
    GenerateChunk(chunkX, chunkY + 1, chunkZ, defaultChunk, false, true);
    GenerateChunk(chunkX, chunkY, chunkZ + 1, defaultChunk, false, true);
    MeshChunk(chunkX, chunkY, chunkZ);
}

void ChunkHandler::RemeshChunk(int chunkX, int chunkY, int chunkZ, bool updateNeighbors) {
    GetChunk(chunkX, chunkY, chunkZ).GenerateMesh(*this, true);

    if (updateNeighbors) {
        GetChunk(chunkX + 1, chunkY, chunkZ).GenerateMesh(*this, true);
        GetChunk(chunkX - 1, chunkY, chunkZ).GenerateMesh(*this, true);
        GetChunk(chunkX, chunkY + 1, chunkZ).GenerateMesh(*this, true);
        GetChunk(chunkX, chunkY - 1, chunkZ).GenerateMesh(*this, true);
        GetChunk(chunkX, chunkY, chunkZ + 1).GenerateMesh(*this, true);
        GetChunk(chunkX, chunkY, chunkZ - 1).GenerateMesh(*this, true);
    }
}

void ChunkHandler::AddBlock(int chunkX, int chunkY, int chunkZ, int blockX, int blockY, int blockZ, unsigned short newBlockID) {
    Chunk& chunk = GetChunk(chunkX, chunkY, chunkZ);
    Block& block = chunk.blocks[blockX][blockY][blockZ];
    if (block.IsAir() ^ (newBlockID == 0)) {
        int currentBlocks = chunk.GetTotalBlocks();
        if (newBlockID == 0) {
            chunk.SetTotalBlocks(currentBlocks - 1);
        }
        else {
            chunk.SetTotalBlocks(currentBlocks + 1);
        }
    }
    block.SetBlockID(newBlockID);
}

void ChunkHandler::RemoveBlock(int chunkX, int chunkY, int chunkZ, int blockX, int blockY, int blockZ) {
    AddBlock(chunkX, chunkY, chunkZ, blockX, blockY, blockZ, 0);
}

void ChunkHandler::Delete() {
    for (auto it = chunks.begin(); it != chunks.end(); ++it) {
        auto& chunk = it->second;
        chunk.Delete();
        world.totalChunks--;
    }

    chunks.clear();
    world.totalMemoryUsage = 0;
}