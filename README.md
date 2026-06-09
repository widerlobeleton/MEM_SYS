# MEM_SYS
#ORCHESTA CONTROLA TODOS LOS OTROS ALGORITMOS SIENDO EL INICIAL BSP,LOGICAMENTE, DESPUES DE INICIALIZAR LOS PUNTEROS Y EVITAR MEM LEAK CON TSharedPtr SUPER IMPORTANTE TENERLO EN CUENTA, SIGUE CON PUNTEROS LOCALES, SI SE QUIERE MODIFICAR A UNA FUNCION SE LE LLAMA CON & DE LO CONTRARIO SIN ESTO SE LE HACE UN PASS BY VALUE Y SE METE UNA QUASI FUNCION A UN STACK PROPIO DE C++, DE NO HACERLO SE GENERA OTRA FUNCION IGUAL, INCIALIZAR EL ENTORNO CON void 

AMyDungeonOrchestrator::ProcessNextMapChunk(int32 ChunkStartX, int32 ChunkWidth, FRandomStream& Stream)
LLAMAS FUNCIONES EXTERNAS, LIBRERIAS Y EL MISMO ORCHESTA, EL CUAL EN ESTE CASO SIENDO UE SE LLAMA CON A, AHORA MISMO VER 5.7



FUNCIONES IMPORTANTES A APRENDER PARA EFICIENCIA DE C++ EN UE5.7


TSharedPtr 

TMakeShared

for (auto& Leaf : ChunkLeaves): forma MUCHO mas logica que el for de python aut& deja a interpretacion de c++ el tipo de dato 

&& Stream.FRandRange(0.0f, 1.0f) Tener en cuenta que los floats van con f siendo esta su representancion, FRandRange es el randint de c++

GetWorld()->SpawnActor<AActor>; Spawnea objetos en el interprete de UE5.7

C:\Users\wider\source\repos\Orchestraror\Orchestraror