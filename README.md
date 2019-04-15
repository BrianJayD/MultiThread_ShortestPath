# MultiThread ShortestPath
### *Collaborators*
###### Brian Domingo - 100528062
###### Christien Soosaipillai - 100557844


### ***How to run:***
**Simple make command that compiles all C programs**
```
make
```
**Compile Sequential program**
```
make seq
```
**Compile OpenMP program**
```
make omp
```
**Compile POSIX Thread program**
```
make pt
```
**Run executables (after successful compile)**
```
# #.path = '#'
# @ = starting node
# Sequential (replace # and @)
make seq#@

# OpenMP (replace # for file number, @ for starting node)
make omp#@

# POSIX Thread (replace # for file number, @ for starting node)
make pt#@

# Example
make seq1a
make omp2b
make pt3c
```

### *Sample path image references*
**1.path:**

![alt text](https://www.cs.bham.ac.uk/~mdr/teaching/modules04/java2/Dijkstra.gif)
