import os
import platform
import ctypes


class GDChessBot:
    def __init__(self):
        self.library_path = self.get_default_library_path()
        self.library = None

    def load_library(self):
        if not os.path.exists(self.library_path):
            raise FileNotFoundError(f"Library not found at {self.library_path}")

        try:
            self.library = ctypes.CDLL(self.library_path)
            print(f"Successfully loaded library: {self.library_path}")
        except Exception as e:
            raise RuntimeError(f"Failed to load library: {e}")

    @staticmethod
    def get_default_library_path():
        system = platform.system()
        if system == "Windows":
            return os.path.join(os.path.dirname(__file__), ".." "bin", "clibgd_chess.windows.template_debug.x86_64.dll")
        elif system in ["Linux", "Darwin"]:  # Darwin is macOS
            return os.path.join(os.path.dirname(__file__), "..", "bin", "libgd_chess.linux.template_debug.x86_64.so")
        else:
            raise RuntimeError(f"Unsupported platform: {system}")


if __name__ == "__main__":
    # Example usage
    # default_path = GDChessBot.get_default_library_path()
    bot = GDChessBot()

    try:
        bot.load_library()
    except Exception as e:
        print(f"Error: {e}")