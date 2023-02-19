/* *** INCOMPLETE
 * only set() and get() is implemented
 * erase() and more is missing
 * and there are probably many bug not caught in test
 */

#if !defined(con_t) || !defined(val_t) || !defined(key_t)
#error "Define con_t, key_t and val_t before including"
#endif

/* preprocessor magic */
#define Ccat3(a,b) a##_##b
#define Ccat2(a,b) Ccat3(a,b)
#define Cpref(x) Ccat2(con_t,x)

#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>

#define con_node_t Ccat2(con_t, _)

typedef struct con_node_t
{
    int color;
    key_t key;
    val_t value;
    struct con_node_t *parent, *left, *right;
} con_node_t;

typedef struct
{
    size_t size;
    con_node_t *root;
} con_t;

static con_node_t *Cpref(node)(key_t k, val_t v, struct con_node_t *parent,
        int color)
{
    con_node_t *n = (con_node_t*)calloc(1, sizeof *n);
    n->key = k;
    n->value = v;
    n->parent = parent;
    n->color = color;
    return n;
}

/*       Q
 *     /  \
 *    P    b
 *   / \
 *  c  d
 *  
 * Right Rotate on Q
 * result:
 *       P
 *     /  \
 *    c   Q
 *       / \
 *      d   b
 *
 *
 *        X
 *      /  \
 *     a    P
 *         / \
 *        c   d
 *
 * Left Rotate on X
 * result:
 *       P
 *      / \
 *     X   d
 *    / \
 *   a  c
 *
 */

void Cpref(left_rotate)(con_node_t **root, con_node_t *Q)
{
    /* https://en.wikipedia.org/wiki/Tree_rotation */
    assert(Q && root);
    struct con_node_t *P = Q->right;
    Q->right = P->left;
    if (Q->right) Q->right->parent = Q;
    P->parent = Q->parent;
 
    if (P->parent == NULL) *root = P;
    /* adjust Q's parent to point to P instead */
    else if (Q == Q->parent->left) Q->parent->left = P;
    else if (Q == Q->parent->right) Q->parent->right = P;
    else assert(0);

    P->left = Q;
    Q->parent = P;
}

void Cpref(right_rotate)(con_node_t **root, con_node_t *Q)
{
    /* https://en.wikipedia.org/wiki/Tree_rotation */
    assert(Q && root);
    struct con_node_t *P = Q->left;
    Q->left = P->right;
    if (Q->left) Q->left->parent = Q;
    P->parent = Q->parent;
 
    if (P->parent == NULL) *root = P;
    /* adjust Q's parent to point to P instead */
    else if (Q == Q->parent->left) Q->parent->left = P;
    else if (Q == Q->parent->right) Q->parent->right = P;
    else assert(0);

    P->right = Q;
    Q->parent = P;
}

con_t Cpref(init)(void)
{
    con_t m;
    m.size = 0;
    m.root = NULL;
    return m;
}

val_t *Cpref(get)(con_t *m, key_t key)
{
    struct con_node_t *c = m->root;
    if (c == NULL) return NULL;
    while (1) {
        if (key > c->key) {
            if (c->right) {
                assert(c != c->right);
                c = c->right;
            } else {
                return NULL;
            }
        } else if (key < c->key) {
            if (c->left) {
                assert(c != c->left);
                c = c->left;
            } else {
                return NULL;
            }
        } else {
            return &c->value;
        }
    }
}

void Cpref(free_)(con_node_t *n)
{
    if (n) {
        if (n->left) Cpref(free_)(n->left);
        if (n->right) Cpref(free_)(n->right);
        free(n);
    }
}

void Cpref(free)(con_t *m)
{
    Cpref(free_)(m->root);
}

void Cpref(erase)(con_t *m, key_t key)
{

}

void Cpref(set)(con_t *m, key_t key, val_t value)
{
    if (m->root == NULL) {
        m->root = Cpref(node)(key, value, NULL, 0);
        return;
    }

    {
        val_t *p = 0;
        if ((p = Cpref(get)(m, key))) {
            *p = value;
            return;
        }
    }

    /* normal BST insert */
    con_node_t *c = m->root;
    while (1) {
        if (key > c->key) {
            if (c->right) {
                c = c->right;
            } else {
                c = (c->right = Cpref(node)(key, value, c, 1));
                break;
            }
        } else {
            if (c->left) {
                c = c->left;
            } else {
                c = (c->left = Cpref(node)(key, value, c, 1));
                break;
            }
        }
    }

    assert(c->parent);

    /* let P = c->parent and G = P->parent
     * 4 possible cases that need rotation
     * LL - G->left == P and P->left == c
     *      1. Right-rotate G
     *      2. Swap G-P color
     * LR - G->left == P and P->right == c
     *      1. Left-rotate P ( it will become G->left == c , c->left == P )
     *      2. Right-rotate G ( now c->left == P and c->right == G)
     *      3. Swap G-c color
     * RR - G->right == P and P->right == c (LL mirror)
     *      1. Left-rotate G
     *      2. Swap G-P color
     * RL - G->right == P and P->left == c (LR mirror)
     *      1. Right-rotate P
     *      2. Left-rotate G
     *      3. Swap G-c color
     */

    while (c != m->root && c->color == 1 && c->parent->color == 1 && c->parent->parent) {
        struct con_node_t *G = c->parent->parent;
        struct con_node_t *P = c->parent;
        if (G->left == P) { /* both c and parent are RED, need to RECOLOR */
            /* must shadow the outer uncle */
            struct con_node_t *U = G->right;

            if (U && U->color == 1) { /* uncle RED */
                /* change parent and uncle to BLACK  grandparent to RED */
                P->color = U->color = 0;
                G->color = 1;
                c = G;
            } else { /* no uncle or uncle BLACK */
                if (c == P->right) {
                    /* LR */
                    Cpref(left_rotate)(&m->root, P);
                    c = P;
                    P = c->parent;
                }

                Cpref(right_rotate)(&m->root, G);
                int t = P->color;
                P->color = G->color;
                G->color = t;
                c = P;
#if 0
                if (P->left == c) { /* LL */
                    Cpref(right_rotate)(&m->root, G);
                    int t = G->color;
                    G->color = P->color;
                    P->color = t;
                    //c = P;
                } else if (P->right == c) { /* LR */
                    Cpref(left_rotate)(&m->root, P);
                    Cpref(right_rotate)(&m->root, G);
                    int t = G->color;
                    G->color = c->color;
                    c->color = t;
                    //c = P;
                } else {
                    assert(0);
                }
#endif
            } 
        } else if (G->right == P) {
            struct con_node_t *U = G->left;

            if (U && U->color == 1) { /* Uncle RED, need to RECOLOR */
                G->color = 1;
                P->color = U->color = 0;
                c = G;
            } else {
                if (P->left == c) {
                    /* RL */
                    Cpref(right_rotate)(&m->root, P);
                    c = P;
                    P = c->parent;
                }

                Cpref(left_rotate)(&m->root, G);
                int t = P->color;
                P->color = G->color;
                G->color = t;
                c = P;
#if 0
                else if (P->right == c) { /* RR */
                Cpref(left_rotate)(&m->root, G);
                int t = G->color;
                G->color = P->color;
                P->color = t;
                //c = P;
            } else if (P->left == c) { /* RL */
                Cpref(right_rotate)(&m->root, P);
                Cpref(left_rotate)(&m->root, G);
                int t = G->color;
                G->color = c->color;
                c->color = t;
            } else {
                assert(0);
            }
#endif
            }
        } else {
            assert(0);
        }

        if (m->root != c) assert(c->parent);
    }

    m->root->color = 0;
}

#undef Ccat3
#undef Ccat2
#undef Cpref
#undef con_node_t
#undef con_t
#undef key_t
#undef val_t


