number=100 # number of simulated individuals
R=3.0 #Gamma+/Gamma-
gammap=7.5 #damage rate constant
gamman=6.5 #repair rate constant
N=10000 #network size
nd=2 #number of mortality nodes
alpha=2.27 #scale free exponent
avgdeg=4 #average degree (must be even for scalefree and smallworld)
Mortality="AND" #mortality condition: "OR" or "AND"
Topology="Single" #"Random" generates a new sampled topology for each individual or "Single" for the same network for all simulated individuals
Folder="Local" #"Local" uses the Data/ folder. Otherwise use scratch space on a computing cluster.
NetworkType="ScaleFree" #"ScaleFree", or "Random", or "SmallWorld".
SingleSeed=1 #Seed used to make the single topology
pA=0.0 #if > 0, will modify the assortativity of the network. https://en.wikipedia.org/wiki/Assortativity
RateIncrease=5.0
StartTime=0.0549
tau=0.0183
Tag=0

make

#ID of this run with seed found from SeedFile. (Row number TaskID will be the seed).
#Change this loop here to do multiple runs with different seeds at once.
for TaskID in `seq -f "%.0f" 1 1`
do	
    ./main $number $R $gammap $gamman $N $nd $alpha $avgdeg $Mortality $Topology $Folder $NetworkType $TaskID $SingleSeed $pA $RateIncrease $StartTime $tau $Tag
done
