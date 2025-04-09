import os
import platform
import ctypes

# need to explicitly define the class type of the ChessUCI C++ class that gets returned to prevent seg-fault
class ChessUCI(ctypes.Structure):
    _fields_ = [
        ("chessBot", ctypes.POINTER(ctypes.c_void_p))  # Adjust to actual member types of ChessUCI
    ]

class GDChessBot:
    def __init__(self):
        self.library_path = self.get_default_library_path()
        self.library = None
        self.load_library()
        self.uci_instance = None
        if not self.library:
            raise RuntimeError("Library not loaded. Call load_library() first.")
        self.library.createChessUci.restype = ctypes.POINTER(ChessUCI)
        self.uci_instance = self.library.createChessUci()
        if not self.uci_instance:
            raise RuntimeError("Failed to create UCI instance.")

    def load_library(self):
        if not os.path.exists(self.library_path):
            raise FileNotFoundError(f"Library not found at {self.library_path}")

        try:
            self.library = ctypes.CDLL(self.library_path)
            print(f"Successfully loaded library: {self.library_path}")
        except Exception as e:
            raise RuntimeError(f"Failed to load library: {e}")
        

    def input_command(self, command: str) -> str:
        if not self.uci_instance:
            raise RuntimeError("UCI instance not created.")
        self.library.inputCommand.restype = ctypes.c_char_p
        return self.library.inputCommand(self.uci_instance, command.encode()).decode()

    def input_fen(self, fen: str):
        print('gonna run input fen')
        if not self.uci_instance:
            raise RuntimeError("UCI instance not created.")
        self.library.inputFEN.argtypes = [ctypes.POINTER(ChessUCI),  ctypes.c_char_p]
        self.library.inputFEN(self.uci_instance, fen.encode())
        print('python input fen working')

    def export_fen(self) -> str:
        if not self.uci_instance:
            raise RuntimeError("UCI instance not created.")
        self.library.exportFEN.restype = ctypes.c_char_p
        return self.library.exportFEN(self.uci_instance).decode()

    def get_bot_move(self, search_depth: int, time_limit: int) -> str:
        if not self.uci_instance:
            raise RuntimeError("UCI instance not created.")
        self.library.getBotMove.argtypes = [ctypes.POINTER(ChessUCI), ctypes.c_short, ctypes.c_int]
        self.library.getBotMove.restype = ctypes.c_char_p
        return self.library.getBotMove(self.uci_instance, search_depth, time_limit).decode()

    def validate_move(self, move: str) -> bool:
        if not self.uci_instance:
            raise RuntimeError("UCI instance not created.")
        self.library.validateMove.restype = ctypes.c_bool
        return self.library.validateMove(self.uci_instance, move.encode())

    def make_move(self, move: str):
        if not self.uci_instance:
            raise RuntimeError("UCI instance not created.")
        self.library.makeMove.argtypes = [ctypes.POINTER(ChessUCI),  ctypes.c_char_p]
        self.library.makeMove(self.uci_instance, move.encode())

    def set_option(self, option: str, value: str):
        if not self.uci_instance:
            raise RuntimeError("UCI instance not created.")
        self.library.setOption(self.uci_instance, option.encode(), value.encode())

    def get_move_history(self) -> list:
        if not self.uci_instance:
            raise RuntimeError("UCI instance not created.")
        self.library.getMoveHistoryPtr.restype = ctypes.POINTER(ctypes.c_char_p)
        move_history = self.library.getMoveHistoryPtr(self.uci_instance)
        moves = []
        for i in range(100):
            move = move_history[i]
            if not move:
                break
            moves.append(move.decode())
        self.library.freeMoveHistoryPtr(self.uci_instance, move_history)
        return moves

    def get_game_result(self) -> int:
        if not self.uci_instance:
            raise RuntimeError("UCI instance not created.")
        self.library.getGameResult.restype = ctypes.c_short
        result = self.library.getGameResult(self.uci_instance)
        if result == 0:
            return "normal move"
        elif result == 1:
            return "check"
        elif result == 2:
            return "1 - 0"
        elif result == 3:
            return "0 - 1"
        elif result == 4:
            return "0.5 - 0.5"
        else:
            assert False, f"unknown game result {result}"
        

    def __del__(self):
        if self.uci_instance:
            self.library.destroyChessUci(self.uci_instance)
            self.uci_instance = None

    @staticmethod
    def get_default_library_path():
        system = platform.system()
        if system == "Windows":
            return os.path.join(os.path.dirname(__file__), "..", "build", "libgd_chess.windows.template_debug.x86_64.dll")
        elif system in ["Linux", "Darwin"]:  # Darwin is macOS
            return os.path.join(os.path.dirname(__file__), "..",  "build", "libgd_chess.linux.template_debug.x86_64.so")
        else:
            raise RuntimeError(f"Unsupported platform: {system}")


if __name__ == "__main__":
    # Example usage
    # default_path = GDChessBot.get_default_library_path()
    bot = GDChessBot()

    try:
        # bot.load_library()
        pass
    except Exception as e:
        print(f"Error: {e}")