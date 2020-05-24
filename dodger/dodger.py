import ctypes
import threading
import time
from open_close import OpenClose


class Dodger(OpenClose):
    """Contains all the necessary glue logic to make Dodger work"""
    
    def __init__(self):
        self._lib = ctypes.cdll.LoadLibrary("../bazel-bin/src/libdodger.so")

        # Return type specification, ctypes defaults to int otherwise
        self._lib.lidar_get_distance_cm.restype = ctypes.c_uint16
        self._lib.lidar_get_distance_filt_cm.restype = ctypes.c_double
    
    def dodge(self):
        for _ in range(0, 10000):
            print(self.lidar_distance_cm, self.lidar_distance_filt_cm)
            time.sleep(0.1)

    @property
    def lidar_distance_cm(self):
        return self._lib.lidar_get_distance_cm()
    
    @property
    def lidar_distance_filt_cm(self):
        return self._lib.lidar_get_distance_filt_cm()        

    def open(self):
        self.lidar = threading.Thread(
            target=self._lib.lidar_poll_1kHz,
            daemon=True
        )
        self.lidar.start()

        return self

    def close(self):
        pass


if __name__ == "__main__":
    with Dodger() as dodger:
        dodger.dodge()
