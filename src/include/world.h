#ifndef WORLD_H
#define WORLD_H

#include "util.h"
#include "gfx.h"
#include "chunk.h"
#include "block.h"
#include "player.h"

struct WorldUnloadedData {
    ivec3s pos;
    u32 data;
};

struct World {
    struct EntityPlayer player;

    // Size of one dimension of World::chunks
    size_t chunks_size;

    // Array of chunks in this world, size is always chunks_size ^ 2
    // Chunk pointers canbe NULL if they are not yet loaded 
    struct Chunk **chunks;

    // Bottom-left offset of chunks array and offset of the center chunk
    ivec3s chunks_origin, center_offset;

    // Data which has been set in the world (i.e. by the world generator)
    // but is not yet in a loaded chunk
    struct {
        struct WorldUnloadedData *list;
        size_t size, capacity;
    } unloaded_data;

    // Per-frame throttles on certain world operations
    struct {
        struct {
            size_t count, max;
        } load, mesh;
    } throttles;
};

// see worldgen.c
void worldgen_generate(struct Chunk *);

bool world_in_bounds(struct World *self, ivec3s pos);
bool world_contains(struct World *self, ivec3s pos);
struct Chunk *world_get_chunk(struct World *self, ivec3s offset);
ivec3s world_pos_to_block(vec3s pos);
ivec3s world_pos_to_offset(ivec3s pos);
ivec3s world_pos_to_chunk_pos(ivec3s pos);

void world_init(struct World *self);
void world_destroy(struct World *self);

void world_remove_unloaded_data(struct World *self, size_t i);

void world_set_data(struct World *self, ivec3s pos, u32 data);
u32 world_get_data(struct World *self, ivec3s pos);
void world_set_center(struct World *self, ivec3s center_pos);
void world_render(struct World *self);
void world_update(struct World *self);
void world_tick(struct World *self);

#endif