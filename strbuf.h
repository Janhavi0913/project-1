typedef struct {
    size_t length;
    size_t used;
    char *data;
} strbuf_t;

int sb_init(strbuf_t *, size_t);
void sb_destroy(strbuf_t *);
int sb_append(strbuf_t *, const char);
int sb_remove(strbuf_t *, char *);
int sb_insert(strbuf_t *list, int index, const char item);
int sb_concat(strbuf_t *sb, char *str);
int sb_extend(strbuf_t *sb, int len);
int max(int val1,int val2);
