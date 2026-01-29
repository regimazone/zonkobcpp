# Regimazone HyperGraphQL OrgAware Identity Framework

## Overview

The Regimazone HyperGraphQL OrgAware Identity Framework is a graph-based identity management system that provides organizational context awareness and hypergraph query capabilities. It extends the existing llama.cpp/koboldcpp graph infrastructure with identity tracking and relationship management.

## Build System Integration

The framework is fully integrated into the koboldcpp build system:

- **Source Files**: `src/llama-hypergraph-identity.cpp` is included in the `GPTTYPE_ADAPTER` build target
- **Tests**: Run `make tests` to build and run `test-hypergraph-identity`
- **Examples**: Run `make examples` to build `hypergraph-identity-demo`
- **Standalone Builds**: Tests and examples can be built independently without requiring the full ggml library

Quick start:
```bash
# Build and run tests
make test-hypergraph-identity
./test-hypergraph-identity

# Build and run example
make hypergraph-identity-demo
./hypergraph-identity-demo
```

## Architecture

### Core Components

1. **Identity Nodes** (`hypergraph_identity_node`)
   - Unique identifier (ID)
   - Type classification (NODE, EDGE, HYPEREDGE, ORG_CONTEXT)
   - Name and attributes
   - Extensible key-value attribute storage

2. **HyperEdges** (`hypergraph_identity_edge`)
   - Connect multiple identity nodes (not just pairs)
   - Named relationship types
   - Support for complex multi-node relationships

3. **Organizational Context** (`org_context`)
   - Organization name
   - Awareness levels (NONE, BASIC, CONTEXTUAL, HIERARCHICAL)
   - Context-specific metadata

4. **Framework Manager** (`hypergraph_identity_framework`)
   - Central management of identities and edges
   - Query operations
   - Org-aware access control
   - Optional GGML tensor integration

### Identity Types

```cpp
enum class identity_type {
    NODE_IDENTITY,           // Basic graph node
    EDGE_IDENTITY,           // Edge representation
    HYPEREDGE_IDENTITY,      // Hyperedge representation
    ORG_CONTEXT_IDENTITY,    // Organizational context
};
```

### Organizational Awareness Levels

```cpp
enum class org_awareness_level {
    NONE,          // No organizational filtering
    BASIC,         // Basic org membership checks
    CONTEXTUAL,    // Context-aware access control
    HIERARCHICAL,  // Hierarchical organization structure
};
```

## Integration with llama.cpp Graph System

The framework integrates with the existing `llm_graph_context` structure:

1. **Header Integration**: `llama-graph.h` includes the hypergraph identity framework
2. **Context Field**: `llm_graph_context` has an optional `hgif` pointer
3. **Parameter Passing**: `llm_graph_params` can carry the framework instance
4. **Tensor Operations**: Optional GGML tensor creation for graph operations

## API Reference

### Identity Management

```cpp
// Add a new identity node
int64_t add_identity_node(identity_type type, const std::string & name);

// Remove an identity node (cascades to connected edges)
bool remove_identity_node(int64_t id);

// Get identity node details
hypergraph_identity_node * get_identity_node(int64_t id);
```

### HyperEdge Management

```cpp
// Create a hyperedge connecting multiple nodes
int64_t add_hyperedge(const std::vector<int64_t> & node_ids, 
                      const std::string & relation_type);

// Remove a hyperedge
bool remove_hyperedge(int64_t id);

// Get hyperedge details
hypergraph_identity_edge * get_hyperedge(int64_t id);
```

### Query Operations

```cpp
// Find all identities of a specific type
std::vector<int64_t> query_identities_by_type(identity_type type);

// Find all identities connected to a given node
std::vector<int64_t> query_connected_identities(int64_t node_id);

// Find all identities involved in a specific relation type
std::vector<int64_t> query_by_relation_type(const std::string & relation_type);
```

### Organizational Context

```cpp
// Set the current organizational context
void set_org_context(const org_context & ctx);

// Get the current organizational context
org_context get_org_context() const;

// Validate org-aware access to an identity
bool validate_org_aware_access(int64_t node_id) const;

// Get all identities accessible in current org context
std::vector<int64_t> get_org_scoped_identities() const;
```

### GGML Integration

```cpp
// Create a tensor representing an identity node
ggml_tensor * build_identity_tensor(ggml_context * ctx, int64_t node_id);

// Create an adjacency matrix tensor for the hypergraph
ggml_tensor * build_hypergraph_adjacency_tensor(ggml_context * ctx);
```

## Usage Examples

### Basic Usage

```cpp
#include "llama-hypergraph-identity.h"

// Create framework
regimazone::hypergraph_identity_framework framework;

// Set org context
regimazone::org_context ctx("my-org", regimazone::org_awareness_level::CONTEXTUAL);
framework.set_org_context(ctx);

// Add identities
int64_t alice = framework.add_identity_node(
    regimazone::identity_type::NODE_IDENTITY, "alice");
int64_t bob = framework.add_identity_node(
    regimazone::identity_type::NODE_IDENTITY, "bob");

// Create relationship
int64_t edge = framework.add_hyperedge({alice, bob}, "collaborates_with");

// Query
auto collaborators = framework.query_by_relation_type("collaborates_with");
```

### Integration with Graph Context

```cpp
// In graph building code
llm_graph_params params;
params.hgif = &framework;  // Pass framework instance

// Framework is now accessible in llm_graph_context
// context.hgif->query_identities_by_type(...)
```

## Implementation Details

### Memory Management

- Uses `std::unique_ptr` for automatic memory management
- No manual memory allocation required
- Cascade deletion: removing a node also removes connected edges

### ID Assignment

- Sequential ID assignment starting from 0
- Separate ID spaces for nodes and edges
- IDs are never reused within a session

### Thread Safety

**Note**: The current implementation is not thread-safe. If using in a multi-threaded environment, external synchronization is required.

## Testing

The framework includes comprehensive unit tests covering:

1. Basic node creation and retrieval
2. Node removal and cascade deletion
3. Hyperedge creation and management
4. Query operations (by type, by connection, by relation)
5. Organizational context management

Run tests:
```bash
# Using the Makefile
make test-hypergraph-identity
./test-hypergraph-identity

# Or build manually
g++ -std=c++17 -I. -Iggml/include -Isrc \
    tests/test-hypergraph-identity.cpp \
    src/llama-hypergraph-identity.cpp \
    -o test-hypergraph-identity
./test-hypergraph-identity
```

## Example Application

See `examples/hypergraph-identity/` for a complete demonstration application.

Run the example:
```bash
# Using the Makefile
make hypergraph-identity-demo
./hypergraph-identity-demo

# Or build manually
g++ -std=c++17 -I. -Iggml/include -Isrc \
    examples/hypergraph-identity/hypergraph-identity.cpp \
    src/llama-hypergraph-identity.cpp \
    -o hypergraph-identity-demo
./hypergraph-identity-demo
```

## Future Extensions

Potential enhancements:

1. **Persistence**: Save/load graph state to disk
2. **Advanced Queries**: Graph traversal, shortest paths, community detection
3. **Performance**: Indexing for faster queries
4. **Security**: Enhanced org-aware access control with ACLs
5. **GGML Integration**: Full tensor operation support for graph algorithms
6. **Distributed Graphs**: Support for distributed hypergraph structures
7. **Schema Validation**: Type checking and constraint enforcement

## Files

- `src/llama-hypergraph-identity.h` - Header file with declarations
- `src/llama-hypergraph-identity.cpp` - Implementation
- `examples/hypergraph-identity/` - Example application
- `tests/test-hypergraph-identity.cpp` - Unit tests
- `docs/hypergraph-identity-framework.md` - This documentation
