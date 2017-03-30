static const char *s = "abc";
int main() {
    return !(*s == 'a' && s++ && *s == 'b' && s++ && *s == 'c' && s++ && *s == '\0');
}
