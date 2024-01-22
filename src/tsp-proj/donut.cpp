//8729BF7B2234189B8DF8A6DD31770D2B18569C27


#include <getopt.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>
#include <iomanip>
#include <random>


using namespace std;


//process the command line



    enum class Location
    {
        us,
        canada,
        border
    };




    struct Coord
    {
        int x =0;
        int y =0;
        Location location;
    };


    struct MST_struct
    {
        bool k = false;
        double d = numeric_limits<double>::infinity();
        int p =-1;
    };


    struct FASTTSP_struct
    {
        bool k = false;
    };


    double squared_distance(int x_a, int y_a, int x_b, int y_b)
    {   double x_adub = static_cast<double>(x_a);
        double x_bdub = static_cast<double>(x_b);
        double y_adub = static_cast<double>(y_a);
        double y_bdub = static_cast<double>(y_b);
        double x_dist = x_adub - x_bdub;
        double y_dist = y_adub - y_bdub;
        return x_dist*x_dist + y_dist*y_dist;
    }


    class Commandline
    {


        public:
            Commandline()
                    {}


            void getMode(int argc, char * argv[])
            {
               
               
                option long_options[] =
                    {
                    // Fill in two lines, for the "mode" ('m') and
                    // the "help" ('h') options.
                   
                    {"mode", required_argument, nullptr, 'm'},
                    {"help", no_argument, nullptr, 'h'},
                    { nullptr, 0, nullptr, '\0' },
                    };
                // These are used with getopt_long()
                int choice;
                int index = 0;
                opterr = false;


                    // Fill in the double quotes, to match the mode and help options.
                while ((choice = getopt_long(argc, argv, "m:h", long_options, &index)) != -1)
                {
                    switch (choice)
                    {
                        case 'm':
                        {
                            string arg{optarg};
                            if(arg =="MST")
                            {
                                mode = arg;
                            }
                            else if(arg == "FASTTSP")
                            {
                                mode = arg;
                            }
                            else if(arg =="OPTTSP")
                            {
                                mode = arg;
                            }
                            else
                            {
                                cerr <<"improper required argument"<<endl;
                                exit(1);
                            }
                            break;
                        }
                        case 'h':
                        {
                            cout<<"donut program requires arguments MST|FASTTSP|OPTTSP"<<endl;
                            exit(0);
                        }
                        default:
                            cerr<<"Unknown Option"<<endl;
                            exit(1);
                    }  // switch ..choice
                }  // while
            }
        string get_search_mode()
            {
                return mode;
            }


     


        private:
            string mode;
           
    };


    class Donut
    {


        public:
            Donut()
                :num_verticies(0),path_cost(0),best_cost(0)
                {


                }


           


            void read_input()
            {
                cin>>num_verticies;
                int x_coord;
                int y_coord;
                while(cin>>x_coord>>y_coord)
                {
                    Coord dummy;
                    dummy.x = x_coord;
                    dummy.y = y_coord;
                    if(x_coord>0 && y_coord>0)
                    {
                        dummy.location = Location::canada;
                    }
                    else if((x_coord>=0 && y_coord==0) ||(x_coord==0 && y_coord>=0))
                    {
                        dummy.location = Location::border;
                    }
                    else
                    {
                        dummy.location = Location::us;
                    }
                    coords.push_back(dummy);
                   
                }
            }


            void mst()
            {  
                int coords_size = static_cast<int>(coords.size());
                msts.resize(coords_size);
                msts[0].d = 0;
                for(int count=0;count<num_verticies; ++count)
                {
                    int smallest_ind = count;
                    double smallest_val = numeric_limits<double>::max();
                    for(int i =0;i<coords_size;++i)
                    {
                        if(smallest_val>msts[i].d &&msts[i].k ==false)
                        {
                            smallest_val = msts[i].d;
                            smallest_ind = i;
                        }
                    }
                    msts[smallest_ind].k = true;
                    for(int i=0;i<coords_size;++i)
                    {
                        bool able = true;
                        if(coords[smallest_ind].location ==Location::us)
                        {
                            if(coords[i].location == Location::canada)
                            {
                                able = false;
                            }
                        }
                        else if(coords[smallest_ind].location ==Location::canada)
                        {
                            if(coords[i].location==Location::us)
                            {
                                able = false;
                            }
                        }
                        if(able ==true)
                        {
                            double squared_dist = squared_distance(coords[smallest_ind].x,coords[smallest_ind].y,
                            coords[i].x,coords[i].y);
                            if(squared_dist<msts[i].d * msts[i].d && smallest_ind!=i && msts[i].k==false)
                            {
                                msts[i].d = sqrt(squared_dist);
                                msts[i].p = smallest_ind;
                            }
                        }
                       
                    }
                   
                }
                 double length = 0;
                    for(int i =1; i<coords_size;++i)
                    {
                        length+= msts[i].d;
                    }
                    if (length ==numeric_limits<double>::infinity())
                    {
                        cerr<<"Cannot construct MST"<<endl;
                        exit(1);
                    }
                    cout<<length<<"\n";
                    for(int i =1; i<coords_size;++i)
                    {
                        if(i<msts[i].p)
                        {
                            cout<<i<<" "<<msts[i].p<<"\n";
                        }
                        else
                        {
                            cout<<msts[i].p<<" "<<i<<"\n";
                        }
                    }
            }
       
        void fast_tsp(Commandline & line)
        {
            int size = static_cast<int>(coords.size());
            fasts.resize(size);
            tour.push_back(0);
            fasts[0] = true;
            
            for(int count = 1;count<size;++count)
            {
                int city_ind = random_city();
                if(count==1)
                {
                    tour.push_back(city_ind);
                    fasts[city_ind] = true;
                }
                else
                {
                    int smallest_ind = 0;
                    double smallest_dist = numeric_limits<double>::max();
                    for(int i =0; i<static_cast<int>(tour.size())-1;++i)
                    {
                        double current_dist = sqrt(squared_distance(coords[city_ind].x,coords[city_ind].y,coords[tour[i]].x,coords[tour[i]].y))
                        + sqrt(squared_distance(coords[city_ind].x,coords[city_ind].y,coords[tour[i+1]].x,coords[tour[i+1]].y))-sqrt(squared_distance(coords[tour[i]].x,coords[tour[i]].y,coords[tour[i+1]].x,coords[tour[i+1]].y));
                        if(current_dist<smallest_dist)
                        {
                            smallest_dist = current_dist;
                            smallest_ind = i;
                        }
                    }
                    double current_dist = sqrt(squared_distance(coords[city_ind].x,coords[city_ind].y,coords[tour[static_cast<int>(tour.size())-1]].x,coords[tour[static_cast<int>(tour.size())-1]].y))
                        + sqrt(squared_distance(coords[city_ind].x,coords[city_ind].y,coords[0].x,coords[0].y))-sqrt(squared_distance(coords[tour[static_cast<int>(tour.size())-1]].x,coords[tour[static_cast<int>(tour.size())-1]].y,coords[0].x,coords[0].y));
                    if(current_dist<smallest_dist)
                    {
                        smallest_ind = static_cast<int>(tour.size())-1;
                    }
                   
                    fasts[city_ind] = true;
                    tour.insert(tour.begin()+1+smallest_ind,city_ind);
                }
                
                 
            }
          
            best_cost = 0;
            for(int i = 0;i<size-1;++i)
            {
                best_cost +=sqrt(squared_distance(coords[tour[i]].x,coords[tour[i]].y,coords[tour[i+1]].x,coords[tour[i+1]].y));
            }
            best_cost += sqrt(squared_distance(coords[tour[0]].x,coords[tour[0]].y,coords[tour[size-1]].x,coords[tour[size-1]].y));
            if(line.get_search_mode()=="FASTTSP")
            {
                cout<<best_cost<<"\n";
                for(int i = 0;i<size;++i)
                {
                    cout<<tour[i]<<" ";
                }
            }
            best_path = tour;
            
        }

        void opt_tsp(Commandline &line)
        {

            fast_tsp(line);
            //   best_cost = 336.74;
            // tour = {0,1,2,3,4,5,6,7,8,9,10};
            //cout << fixed << showpoint << setprecision(2) << boolalpha;
            path_cost = 0;
            genPerms(1);
            cout<<best_cost<<"\n";
            for(size_t i =0; i <best_path.size();++i)
            {
                cout<<best_path[i]<<" ";
            }
        }
        
       
        private:
            int num_verticies;
            vector<Coord> coords;
            vector<MST_struct> msts;
            vector<bool> fasts;
            //part c
            vector<int> tour;
             vector<int> best_path;
            double path_cost;
            double best_cost;
        
        
       
            void genPerms( size_t permLength) {
            if (permLength == tour.size()) {
            // Do something with the path
                double last_edge_cost= sqrt(squared_distance(coords[tour[0]].x,coords[tour[0]].y,
                coords[tour[static_cast<int>(tour.size())-1]].x,coords[tour[static_cast<int>(tour.size())-1]].y));
                path_cost += last_edge_cost;
                if(path_cost<best_cost)
                {
                    best_cost = path_cost;
                    best_path = tour;
                    //cout<<"New best cost achieved: "<<best_cost<<"\n";
                }
                path_cost -= last_edge_cost;
                return;
            }  // if ..complete path
            if (!promising(permLength)) {
                return;
            }  // if ..not promising, return

            for (size_t i = permLength; i < tour.size(); ++i) {
                swap(tour[permLength], tour[i]);
                double new_length = sqrt(squared_distance(coords[tour[permLength-1]].x,coords[tour[permLength-1]].y,
                coords[tour[permLength]].x,coords[tour[permLength]].y));
                path_cost += new_length;
                genPerms(permLength + 1);
                path_cost -= new_length;
                swap(tour[permLength], tour[i]);
            }  // for ..unpermuted elements
            }  // genPerms()

            bool promising(size_t permLength)
            {

                 size_t k = tour.size()-permLength;
                 if(k<5)
                 {
                    return true;
                 }
              
                vector<int> subtour(tour.begin()+permLength,tour.end());
                int subtour_size = static_cast<int>(subtour.size());
                msts.clear();
                msts.resize(subtour_size);
                msts[0].d = 0;
                for(int count=0;count<subtour_size; ++count)
                {
                    int smallest_ind = count;
                    double smallest_val = numeric_limits<double>::max();
                    for(int i =0;i<subtour_size;++i)
                    {
                        if(smallest_val>msts[i].d &&msts[i].k ==false)
                        {
                            smallest_val = msts[i].d;
                            smallest_ind = i;
                        }
                    }
                    msts[smallest_ind].k = true;
                     for(int i=0;i<subtour_size;++i)
                    {
                            double squared_dist = squared_distance(coords[subtour[smallest_ind]].x,coords[subtour[smallest_ind]].y,
                            coords[subtour[i]].x,coords[subtour[i]].y);
                            if(squared_dist<msts[i].d * msts[i].d && smallest_ind!=i && msts[i].k==false)
                            {
                                msts[i].d = sqrt(squared_dist);
                                msts[i].p = smallest_ind;
                            }
                    }
                }
                 double mstCost = 0;
                for(int i =1; i<subtour_size;++i)
                    {
                        mstCost+= msts[i].d;
                    }
                double first_length = numeric_limits<double>::max();
                double lasts_length = numeric_limits<double>::max();
                // if(permLength==1)
                // {
                //     for(int i =0; i<subtour_size;++i)
                //     {
                //         double squared_dist1 = squared_distance(coords[tour[0]].x,coords[tour[0]].y,
                //                 coords[subtour[i]].x,coords[subtour[i]].y);
                //         if(squared_dist1<first_length)
                //         {
                //             lasts_length = first_length;
                //             first_length = squared_dist1;
                //         }
                //         else if(squared_dist1<lasts_length)
                //         {
                //             lasts_length = squared_dist1;
                //         }
                //     }
                // }
               
                    for(int i =0; i<subtour_size;++i)
                    {
                        double squared_dist1 = squared_distance(coords[tour[0]].x,coords[tour[0]].y,
                                coords[subtour[i]].x,coords[subtour[i]].y);
                        double squared_dist2 = squared_distance(coords[tour[permLength-1]].x,coords[tour[permLength-1]].y,
                                coords[subtour[i]].x,coords[subtour[i]].y);
                        if(squared_dist1<first_length)
                        {
                            first_length = squared_dist1;
                        }
                        if(squared_dist2<lasts_length)
                        {
                            lasts_length = squared_dist2;
                        }
                    }
                
                if(permLength==1)
                {
                    lasts_length = first_length;
                }
                first_length = sqrt(first_length);
                lasts_length = sqrt(lasts_length);
                  double total = path_cost+ first_length + lasts_length + mstCost;
                //   for (size_t i = 0; i < tour.size(); ++i)
                // {
                //     cout << setw(2) << tour[i] << ' ';
                // }
              
                // cout << setw(4) << permLength << setw(10) << path_cost;
                // cout << setw(10) << first_length << setw(10) << lasts_length;
                // cout << setw(10) << mstCost << setw(10) << total << "  ";
                // cout << ((path_cost+ first_length + lasts_length + mstCost) < best_cost) << '\n';
               
                if(total>=best_cost)
                {
                    return false;
                }
                return true;
            }

         

       
            int random_city()
            {
                int city = rand()%static_cast<int>(coords.size());
                while(fasts[city]==true)
                {
                    //city = rand()%static_cast<int>(coords.size());
                    if(city ==static_cast<int>(coords.size())-1)
                    {
                        city = 0;
                    }
                    else
                    {
                        ++city;
                    }
                    
                }


                return city;
            }
    };


    int main(int argc, char* argv[])
    {
        ios_base::sync_with_stdio(false);
        cout << std::setprecision(2); //Always show 2 decimal places
        cout << std::fixed; //Disable scientific notation for large numbers
        Commandline line;
        line.getMode(argc,argv);
        Donut nut;
        nut.read_input();
        if(line.get_search_mode()=="MST")
        {
            nut.mst();
        }
        if(line.get_search_mode()=="FASTTSP")
        {
            nut.fast_tsp(line);
        }
        if(line.get_search_mode()=="OPTTSP")
        {
            nut.opt_tsp(line);
        }
       


    }
