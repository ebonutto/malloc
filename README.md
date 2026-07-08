# ft_malloc

> Custom implementation of `malloc`, `free`, and `realloc` in C — dynamically loaded via `LD_PRELOAD`.

---

## Architecture

```
.
├── include
│   ├── malloc.h        # Public API
│   └── malloc_int.h    # Internal structs, macros, prototypes
├── src
│   ├── alloc           # Zone and chunk management
│   ├── env             # Environment variable parsing
│   ├── history         # Allocation history
│   ├── malloc
│   └── show            # show_alloc_mem / show_alloc_mem_ex
└── tests
    └── main.c
```

---

## How it works

Memory is split into three categories based on requested size :

| Category | Size     | Strategy                           |
|----------|----------|------------------------------------|
| TINY     | ≤ 128 B  | Pre-allocated zones (~16 KB each)  |
| SMALL    | ≤ 1024 B | Pre-allocated zones (~128 KB each) |
| LARGE    | > 1024 B | Dedicated `mmap` per allocation    |

### Zone layout

```
mmap() :
┌────────────┬────────────┬────────────┬────────────┬────────────┐
│  t_zone    │  t_chunk   │    data    │  t_chunk   │    data    │
│  (header)  │  (header)  │            │  (header)  │            │
└────────────┴────────────┴────────────┴────────────┴────────────┘
```

Each zone holds multiple chunks. When a zone is entirely free, it is released via `munmap`.

### Chunk splitting

On allocation, a free chunk is split in two — the requested size and a leftover :

```
Before malloc(64) :   [ free : 500 ]
After  malloc(64) :   [ used : 64  ][ free : 404 ]
```

A leftover is only created if it can hold at least a chunk header (32 bytes).
If the remaining space is too small, the entire chunk is given to the user :

```
Before malloc(496) :   [ free : 500 ]
After  malloc(496) :   [ used : 500 ]   ← no split, 4 bytes absorbed
```

### Coalescing

On `free`, adjacent free chunks are merged to reduce fragmentation :

```
Before free(b) :   [ a:used ][ b:used ][ c:free ][ d:used ]
After  coalesce :  [ a:used ][      bc:free     ][ d:used ]
```

---

## Global state

A single global structure holds all allocator state :

```c
typedef struct s_malloc_state {
    pthread_mutex_t lock;     // thread safety
    t_zone          *tiny;    // TINY zone list
    t_zone          *small;   // SMALL zone list
    t_zone          *large;   // LARGE block list
    t_history       history;  // allocation ring buffer
    int             flags;    // env var flags
} t_malloc_state;
```

---

## Build

```bash
# Build the shared library
make

# Build + run tests
make test
```

---

## Usage via LD_PRELOAD

```bash
# Replaces malloc in any program without recompilation
LD_PRELOAD=./libft_malloc.so ./your_program
```

---

## Debug functions

```c
void show_alloc_mem(void);     // Print allocated zones and chunks
void show_alloc_mem_ex(void);  // Extended: verbose + hex dump + allocation history
```

### Example output

```
TINY : 0x102a30000
  --- ZONE : 0x102a30000 : 16384 bytes ---
    --- CHUNK : 0x102a30040 - 0x102a30080 : 64 bytes (free : 0)
    --- CHUNK : 0x102a30080 - 0x102a30400 : 15808 bytes (free : 1)
SMALL : 0x0
LARGE : 0x0
Total : 64 bytes

--- HISTORY (3 entries) ---
#0001 [MALLOC ] 0x102a30050 size=64
#0002 [FREE   ] 0x102a30050
#0003 [MALLOC ] 0x102a30050 size=32
```

---

## Environment variables

| Variable         | Effect                                              |
|------------------|-----------------------------------------------------|
| `MALLOC_HISTORY` | Enable allocation history in `show_alloc_mem_ex`    |
| `MALLOC_HEXDUMP` | Enable hexdump visualization in `show_alloc_mem_ex` |

```bash
MALLOC_HISTORY=1 MALLOC_HEXDUMP=1 LD_PRELOAD=./libft_malloc.so ./your_program
```

---

## Thread safety

All public functions (`malloc`, `free`, `realloc`, `calloc`) (`show_alloc_mem`, `show_alloc_mem_ex`) are protected by a `pthread_mutex_t`. The allocator is safe for multi-threaded use.

---

## Implementation notes

- Zones are aligned to `getpagesize()` for correct `munmap` behavior
- Chunk data is aligned to 16 bytes (SIMD compatibility, ARM requirement)
- Chunk type and free status are stored as bitflags in a single `size_t flags` field
- History is implemented as a **ring buffer** of 16 entries — zero allocation overhead
- `LD_PRELOAD` replaces the system `malloc` entirely — no recompilation needed

---

## Author

**Elio BONUTTO** — School 42 Paris, Cybersecurity & Cryptology Engineer
