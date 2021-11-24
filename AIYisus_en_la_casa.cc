#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Yisus


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }


typedef vector< vector<bool> > Matrix_bool;

struct position {
   Pos pos;
   Dir dir; 
};



Matrix_bool queued;



bool valid_position(int i, int j) {

  Cell act = cell(i, j);

  return act.type != Abyss
     and act.type != Rock 
     and act.type != Granite 
     and not queued[i][j];
}

bool is_monster(int i, int j) {

  Pos p(i,j);
  int n = cell(p).id;
  if (n != -1 and (unit(n).type == Balrog or unit(n).type == Orc or unit(n).type == Troll)) return true;
  else return false;
}
 

Dir bfs(Pos actual, int tropa) {


  if (tropa == 0) {

    queued = Matrix_bool(60, vector<bool>(60, false));


  
    position initial_pos;
    queue<position> q;
    initial_pos.pos = actual;
    queued[actual.i][actual.j] = true;

    int count = 0;
    int i = random(0, 7);

    while(count < 8) {

      Pos aux = initial_pos.pos + Dir(i);

      if (pos_ok(aux.i, aux.j) and valid_position(aux.i, aux.j) and not is_monster(aux.i, aux.j)) {

        int jugador = cell(aux).id;

        if (jugador != -1 and unit(jugador).player != me()) return Dir(i);

        position nueva;
        nueva.pos = aux;
        nueva.dir = Dir(i);
        q.push(nueva);
        queued[aux.i][aux.j] = true;
        
      } 

      ++i;
      ++count;
      if (i == 8) i = 0;

    }


    while (not q.empty()) {
      position current = q.front();
      q.pop();



      int jugador = cell(current.pos).id;

       if (round() < 100) { 
        if (jugador != -1 and unit(jugador).player != me()) return current.dir;
      }
          
      
      if (round() >= 100) {
        if (cell(current.pos.i, current.pos.j).owner != me() /*or cell(current.pos.i, current.pos.j).owner == -1)*/ and cell(current.pos.i, current.pos.j).type != Outside) {

            return current.dir;
            
        }
      }

      i = random(0, 7);
      count = 0;

      while(count < 8) { 

        Pos aux2 = current.pos + Dir(i);

        if (pos_ok(aux2.i, aux2.j) and valid_position(aux2.i, aux2.j) and not is_monster(aux2.i, aux2.j)) {

          position adjacent;
          adjacent.pos = current.pos + Dir(i);
          adjacent.dir = current.dir;
          q.push(adjacent);
          queued[adjacent.pos.i][adjacent.pos.j] = true;
        }
        ++i;
        ++count;
        if (i == 8) i = 0;
      }
    }
    

  }



  if (tropa == 1) {
    queued = Matrix_bool(60, vector<bool>(60, false));


  
    position initial_pos;
    queue<position> q;
    initial_pos.pos = actual;
    queued[actual.i][actual.j] = true;

    for (int i = 0; i < 8; i+= 2) {
      Pos aux = initial_pos.pos + Dir(i);

      if (pos_ok(aux.i, aux.j) and valid_position(aux.i, aux.j)) {

        int jugador = cell(aux).id;

        
        if (jugador != -1 and unit(jugador).player != me() and unit(jugador).type == Dwarf) return Dir(i);

      
        position nueva;
        nueva.pos = aux;
        nueva.dir = Dir(i);
        q.push(nueva);
        queued[aux.i][aux.j] = true;
        
      } 

    }
    while (not q.empty()) {
      position current = q.front();
      q.pop();

      int jugador = cell(current.pos).id;

      if (jugador != -1 and unit(jugador).player == me() and unit(jugador).type == Dwarf) {

          return current.dir;
      }

      for (int i = 0; i < 8; i+= 2) { 

        Pos aux2 = current.pos + Dir(i);

        if (pos_ok(aux2.i, aux2.j) and valid_position(aux2.i, aux2.j) and not is_monster(aux2.i, aux2.j)) {

          position adjacent;
          adjacent.pos = current.pos + Dir(i);
          adjacent.dir = current.dir;
          q.push(adjacent);
          queued[adjacent.pos.i][adjacent.pos.j] = true;
        }
      }
    }
  


  }

  return None;
  
}


 
  
typedef vector<int> VI;

  map<int, int> kind; 

  void move_dwarves() {
    VI D = dwarves(me());
    int n = D.size();
    VI perm = random_permutation(n);
    for (int i = 0; i < n; ++i) {
      int id = D[perm[i]];
      Dir mov = bfs(unit(id).pos, 0);
      command(id, mov);

    }
  }

  void move_wizards() {
    vector<int> W = wizards(me());
    for (int id : W) {
      Dir mov = bfs(unit(id).pos, 1);
      command(id, Dir(mov));
    }
  }


  /**
   * Play method, invoked once per each round.
   */

 
  virtual void play () {
    move_dwarves();
    move_wizards();
  }
};



/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
