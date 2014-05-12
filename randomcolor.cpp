#include "board.h"
#include "randomcolor.h"


RandomColor::RandomColor(Board * board, bool isBiased, double bias)
    : m_board(board)
    , m_isBiased(isBiased)
    , m_bias(bias)
    , m_numColors(board->m_numColors)
{}


void RandomColor::setSeed(int seed){
    m_rng.setSeed(seed);
}

void RandomColor::copyStateRNG(const RandomColor * randcol){
    m_rng = randcol->m_rng;
}

Color RandomColor::gen(int x){
    if(!m_isBiased){
        return Color(1 + m_rng.unifInt(m_numColors));
    } else {
//        cout << "GENERATING " << endl;
//        // Generazione di colori biased con la formula
//        //  p_i = 1 / #col + bias * ( #col * rho_i  - 1)

//        // ATTENZIONE deve essere  0 <= bias < 1 / #col

//        //conta i colori nella tre colonne centrate in x
//        QVector<int> count(m_numColors + 1, 0); // dato che i colori partono da 0
//        int tot = 0;
//        for(int x2 = x - 1; x2 <= x + 1; ++x2){
//            for(int y = 0; y < m_board->gridSize(); ++y){
//                if(m_board->hasDiamond({x2,y})){
//                    auto col = m_board->diamond({x2,y})->color();
//                    if(int(col) >= 1 && int(col) >= 1){ // evito di contare jolly
//                        count[int(col)]++;
//                        tot++;
//                    }
//                }
//            }
//        }

//        cout << "CUMULATING " << endl;
//        //Calcolo la cumulata secondo il bias e confronto con un numero random;
//        double c = 0;
//        int i = 1;
//        double r = m_rng.unifReal();
//        for(; i <= m_numColors; ++i){
//            c +=  1. / m_numColors + m_bias * ( m_numColors * double(count[i]) / tot - 1.);
//            if(r < c) break;
//        }
//        cout << "DONE " << endl;

//        return Color(i);

        return Color(1 + m_rng.unifInt(m_numColors));

    }
}

