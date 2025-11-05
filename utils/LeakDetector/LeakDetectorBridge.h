#ifdef __cplusplus
extern "C" {
#endif

void LeakDetector_Allocated(size_t bytes);
void LeakDetector_Freed(size_t bytes);

#ifdef __cplusplus
}
#endif
