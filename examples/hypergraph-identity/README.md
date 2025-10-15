# Regimazone HyperGraphQL OrgAware Identity Framework

This directory contains an example demonstration of the Regimazone HyperGraphQL OrgAware Identity Framework.

## Overview

The framework provides identity-aware graph query capabilities with organizational context tracking for hypergraph structures. It enables:

- **Identity Management**: Create and manage identity nodes with different types (NODE, EDGE, HYPEREDGE, ORG_CONTEXT)
- **HyperEdge Support**: Connect multiple identities through hyperedges with named relationships
- **Organization-Aware Access**: Control access to identities based on organizational context
- **Query Operations**: Search and filter identities by type, relationships, and organizational scope
- **Graph Integration**: Optional integration with GGML tensor operations for graph computations

## Building

To build the example:

```bash
# Standalone (no GGML integration)
g++ -std=c++17 -I. -Iggml/include -Iggml/src -Iinclude -Isrc \
    examples/hypergraph-identity/hypergraph-identity.cpp \
    src/llama-hypergraph-identity.cpp \
    -o hypergraph-identity-demo
```

## Running

```bash
./hypergraph-identity-demo
```

## Example Output

The demo creates a simple organizational graph with users and relationships, demonstrating:
1. Creating identity nodes
2. Adding hyperedges to connect identities
3. Querying the hypergraph structure
4. Performing org-aware access control
5. Getting statistics and managing the graph

## API Usage

```cpp
#include "llama-hypergraph-identity.h"

// Create framework instance
regimazone::hypergraph_identity_framework framework;

// Set organizational context
regimazone::org_context org_ctx("my-org", regimazone::org_awareness_level::CONTEXTUAL);
framework.set_org_context(org_ctx);

// Add identity nodes
int64_t user_id = framework.add_identity_node(
    regimazone::identity_type::NODE_IDENTITY, 
    "user_alice");

// Create relationships via hyperedges
int64_t edge_id = framework.add_hyperedge(
    {user_id, other_user_id}, 
    "collaborates_with");

// Query the graph
auto connected = framework.query_connected_identities(user_id);
auto by_type = framework.query_identities_by_type(regimazone::identity_type::NODE_IDENTITY);

// Check org-aware access
bool allowed = framework.validate_org_aware_access(user_id);
```

## Integration with GGML

When building as part of the full llama.cpp/koboldcpp project, the framework can create GGML tensors for graph operations. The tensor integration methods (`build_identity_tensor` and `build_hypergraph_adjacency_tensor`) are available when linked with the GGML library.
