#include <iostream>
#include <vector>
#include <fstream>


using namespace std;

struct arete {
  int depart;
  int arrive;
  int cout;
};

//question 2
struct ACM_Data {
  int nombeSommets;
  int nombreArete;
  vector<struct arete> aretes;

};

//question 3
struct ACM_Data createGraphe(string path)
{
  struct ACM_Data graphe;
  ifstream monFlux(path);
  if(monFlux)
  {
      monFlux >> graphe.nombeSommets;
      monFlux >> graphe.nombreArete;
      string line;
      getline(monFlux,line);
      while (getline(monFlux,line)) {
        struct arete tmp;
        tmp.depart = atoi(&line[0])-1;
        tmp.arrive = atoi(&line[2])-1;
        tmp.cout = atoi(&line[4]);
        graphe.aretes.push_back(tmp);
      }
  }
  else
  {
      cout << "Erreur dans la lecture du fichier " << endl;
  }
  return graphe;
}

//question 4
bool operator<(struct arete const & areteA, struct arete const & areteB)
{
  if (areteA.cout < areteB.cout){
    return true;
  }
  else {
    return false;
  }
}

//question 5
void sortSelection(struct ACM_Data& graphe)
{
  int const length = graphe.aretes.size();
  for (int i = 0; i < length - 1; i++) {
    int min = i;
    for (int j = i + 1; j < length; j++) {
      if ( graphe.aretes[j] < graphe.aretes[min])
      {
        min = j;
      }
    }
    if (min != i)
    {
      struct arete tmp = graphe.aretes[i];
      graphe.aretes[i] = graphe.aretes[min];
      graphe.aretes[min] = tmp;
    }
  }
}


//question 6
struct ACM_Solution {
  int coutMinimal;
  vector <struct arete> aretesSolution;
};


//question 7
struct ACM_Solution solve(struct ACM_Data& graphe)
{

  struct ACM_Solution arbreMinimal;
  arbreMinimal.coutMinimal = 0;
  sortSelection(graphe);
  // le tableau qui contient les composantes connexes indixé par les sommets du graphe
  vector<vector<int>*> tableConvexe;
  // initialisation du tableau
  for (int i = 0;  i < graphe.nombeSommets; i++) {
    tableConvexe.push_back(new vector<int>(1,i));
  }

  for (int i = 0; i < graphe.nombreArete; i++) {
    int contientcycle = 0;
    int m = min(graphe.aretes[i].arrive,graphe.aretes[i].depart);
    int l = max(graphe.aretes[i].arrive,graphe.aretes[i].depart);

    //si les deux tableau sont d'intersection non vide
    for (int p = 0; p < (*tableConvexe[m]).size(); p++) {
      for (int j = 0; j < (*tableConvexe[l]).size(); j++) {
        if ((*tableConvexe[m])[p] == (*tableConvexe[l])[j])
        {
          contientcycle = 1;
        }
      }
    }
    if (! contientcycle)
    {
      arbreMinimal.aretesSolution.push_back(graphe.aretes[i]);
      for (int  k = 0; k < (*tableConvexe[l]).size(); k++) {
        tableConvexe[m]->push_back((*tableConvexe[l])[k]);
      }
      delete tableConvexe[l];
      tableConvexe[l] = tableConvexe[m];
    }
    if (arbreMinimal.aretesSolution.size() == graphe.nombeSommets - 1)
    {
      break;
    }
  }

  for (int i = 0; i < arbreMinimal.aretesSolution.size(); i++) {
    arbreMinimal.coutMinimal += arbreMinimal.aretesSolution[i].cout;
  }
  return arbreMinimal;
}



// Pour tester les implémentations
void showGraph(struct ACM_Data const & myGraphe){
  std::cout << myGraphe.nombeSommets << ' ';
  std::cout << myGraphe.nombreArete << '\n';
  for (size_t i = 0; i < myGraphe.aretes.size(); i++) {
    std::cout << myGraphe.aretes[i].depart << ' ';
    std::cout << myGraphe.aretes[i].arrive << ' ';
    std::cout << myGraphe.aretes[i].cout << '\n';
  }
}

void showSolution(struct ACM_Solution const & solution) {
  std::cout << "L'arbre de cout minimal : " << '\n';
  for (int i = 0; i < solution.aretesSolution.size(); i++) {
    std::cout << solution.aretesSolution[i].depart << ' ';
    std::cout << solution.aretesSolution[i].arrive << ' ';
    std::cout << solution.aretesSolution[i].cout << '\n';
  }
  std::cout << "Le cout minimal est : " << solution.coutMinimal <<'\n';
}


int main(int argc, char const *argv[]) {
  string const path = "graphe1.dat";
  struct ACM_Data myGraphe = createGraphe(path);
  cout << "Avant le  Tri : " << '\n';
  showGraph(myGraphe);
  cout << "Après le Tri : " << '\n';
  sortSelection(myGraphe);
  showGraph(myGraphe);
  std::cout << "Résolution par l'algorithme de Kruskal : " << '\n';
  struct ACM_Solution solution = solve(myGraphe);
  showSolution(solution);
  return 0;
}
