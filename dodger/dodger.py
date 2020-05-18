import ctypes


if __name__ == "__main__":
    libdodger = ctypes.cdll.LoadLibrary("../bazel-bin/src/libdodger.so")
    libdodger.foo()