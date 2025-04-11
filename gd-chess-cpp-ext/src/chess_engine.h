#ifndef CHESS_ENGINE_H
#define CHESS_ENGINE_H

#include <godot_cpp/classes/node.hpp>

namespace godot {

    class ChessEngine : public Node {
        GDCLASS(ChessEngine, Node);

        private:
            // Add private members here
            

        protected:
            static void _bind_methods();
            int numCalls = 0;

        public: 

            ChessEngine();
            ~ChessEngine();
            String getFenBoard();
            void setBoardPosition(String fen);
            
            String inputUCI(String command);


            
    };

}



#endif