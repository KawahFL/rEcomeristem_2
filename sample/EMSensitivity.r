#Script sensitivity analysis for EcoMeristem
#Authors : Florian Larue, Gr�gory Beurier, Jean-Christophe Souli�, Lauriane Rouan 
#(PAM, AGAP, BIOS, CIRAD)

##############################Gestion des entr�es-sorties################################
PPath <- "D:/Workspace/R/testestim/"  #chemin vers fichier param�tre et fichier valparam
MPath <- "D:/Workspace/R/testestim/"  #chemin vers fichiers meteo
OVariable <- "biomaero2"             #nom variable � observer
Method <- "F"                         #m�thode AS (M = morris, C = complet, F = fast)
ValParamName <- "ValParam.txt"        #nom fichier valeurs parametres
#########################################################################################


#Install and load packages
list.of.packages <- c("recomeristem","sensitivity","rgl")
new.packages <- list.of.packages[!(list.of.packages %in% installed.packages()[,"Package"])]
if(length(new.packages)) install.packages(new.packages)
lapply(list.of.packages, require, character.only=TRUE)

#Files
parameterFilename <- paste(PPath, "ECOMERISTEM_parameters.txt", sep = "")
ValParamFilename <- paste(PPath, ValParamName, sep = "")
meteo <- recomeristem::getMeteo_from_files(MPath)
parameterFileContents <- recomeristem::getParameters_from_files(PPath)
ValParam <- read.table(ValParamFilename, blank.lines.skip = TRUE, col.names = c("Name", "BInf", "BSup", "Step"))
nbFact <- nrow(ValParam)
ParamAFaireVarier <- as.vector(ValParam[,1])

#Bornes
BornesParamAEstimer <- as.matrix(ValParam[,2:4])
B1 <- BornesParamAEstimer[,1]
B2 <- BornesParamAEstimer[,2]
Step <- BornesParamAEstimer[,3]
NbParam <- length(as.vector(B1))

bounds <- apply(cbind(B1, B2), 1, function(x){list(min = x[1], max = x[2])})

EcoMerist <- function(x) {
  # recherche de l'indice du parametre en cours dans le fichier de parametre
  ind <- match(ParamAFaireVarier,parameterFileContents$Name)
  # substitution de la valeur du parametre
  parameterFileContents$Values[ind] <- x
  write.table(parameterFileContents,
            file = parameterFilename,
            quote = FALSE,
            sep = "=",
            row.names = FALSE,
            col.names = FALSE)
  #On lance la simulation
  EcoSim <- recomeristem::rcpp_run_from_dataframe(parameterFileContents, meteo)
  
  #On r�cup�re LA sortie choisie � l'aide l'interface d'optimisation
  #Attention il faut une seule valeur pas un vecteur
  ValSimTot <- EcoSim[[OVariable]]
  #on r�cup�re la derni�re valeur
  ValSim <- as.numeric(ValSimTot[length(ValSimTot)])
  return(ValSim)
}

EcoMerist.fun <- function(m) {toto=apply(m, 1, EcoMerist); return(toto)}

########################Analyse de Sensibilit�###########################
#M�thode de Morris
SensitivityEcoMerist <- function() {
  sa<-morris(model=NULL,
             factors = ParamAFaireVarier, 
             r = 100, 
             binf=B1, 
             bsup=B2, 
             design = list(type = "oat", levels = 10, grid.jump = 5))
  y <- EcoMerist.fun(sa$X)
  tell(sa,y)
  print(sa)
  plot(sa)
  #plot3d.morris(sa)
}

#PlanComplet
PlanComplet <- function() {
  ListeFacteurs <- vector("list", NbParam)
  for (i in 1:NbParam){
    ListeFacteurs[[i]] <- seq(B1[i], B2[i], Step[i])
  }
  PlanComplet <- expand.grid(ListeFacteurs)
  y <- EcoMerist.fun(PlanComplet)
  sortie <- cbind(PlanComplet, y)
  write.table(sortie, 
              paste(PPath, "sortie_plan_complet.txt", sep = ""), 
              sep="\t", 
              row.names = FALSE,
              col.names = FALSE,
              dec = ".")
}

##M�thode FAST (A ADAPTER)
IndicesSensitivityEcoMerist <- function() {
  sa <- fast99(model = NULL,
             factors = 4,
             n = 450,
             q = rep("qunif", 4),
             q.arg=bounds)
  y <- EcoMerist.fun(sa$X)
  
  tell(sa, y)
  
  sortie <- cbind(sa$X, sa$y)
  write.table(sortie, 
              paste(PPath, "sortie_plan_fast.txt", sep = ""), 
              sep="\t", 
              row.names = FALSE, 
              col.names = FALSE,
              dec = ".")  
  print(sa)
  plot(sa)
}

########################Start Analyse##################################
switch(Method,
       "M" = {
         SensitivityEcoMerist()
       },
       "F" = {
         IndicesSensitivityEcoMerist()
       },
       "C" = {
         PlanComplet()
       }
)
