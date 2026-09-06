# Roadmap

## Milestone inicial - Base para desenvolvimento - done
* Estrutura inicial do projeto
* Configuração de build com CMake
* openGL context 
* Processamento de eventos com SDL3

*Tenho como objetivo ter a aplicação a abrir uma janela, receber input (events) e executar um loop principal simples.*
*A primeira fase começou com a estruturação, configurações iniciais para build com CMake, criação de context com openGL e processamento de eventos para encerramento com SDL3 - após tudo estar operacional, refatorizei o códig para outros ficheiros para evitar ter as funções todas centralizadas.*

## Milestone 1 - Engine Core - done
* Application
* Engine
* Time System
* Event System
* Logging
* Assertions

*Construção do núcleo do motor.*

## Milestone 2 - Renderização 2D - done
* Renderer2D
* Shaders
* Buffers
* Textures
* Camera2D

*Tornar o motor capaz de renderizar texturas/sprites 2D no ecrã.*

## Milestone 3 - Logging - done
* Log Info, Warning, Error
* Exportação para ficheiro externo para consulta
* Log throttling

*Logging de eventos para depuração, exportação para ficheiro texto para consulta e throttle no logging para evitar encher a consola de logs iguais a cada update.*

## Milestone 4 - Matemática - done
* Vector2D, Vector3D
* Transform
* Matrix4x4 - Identity
* Matrix4x4 - Translation
* Matrix4x4 - Scale
* Matrix4x4 - Multiplication
* Matrix4x4 - Ortographic projection


## Milestone 5 - Física 2D - done
* AABB - Collision
* Gravity
* Salto
* Grounded
* Resolução de colisões

*Aplicar matemática para definir físicas em movimento horizonta/vertical e colisões entre objetos.*

## Milestone 6 - Input - done
* Teclado
* Mouse scrolling
* Gampead
* Deadzone

*Implementar sistema de controlo de movimento do jogador, com suporte para gamepad e deadzones para ignorar input desnecessário dentro do joystick.*

## Milestone 7 - Gestão de recursos - done
* Asset manager
* Scene manager

*Tornar o motor capaz de carregar/alternar entre diferentes cenas.*

## Milestone 8 - ECS - done
* Entities
* Components
* Systems 

*Arquitetura baseada em entidades e componentes, assim evitando usar hierarquia em classes.*

## Milestone 9 - Demo 2D (Fixed Camera / Following Camera) - done
* Jogador
* Plataformas e paredes
* Inimigos

*Implementar um mundo básico funcional em que o jogador tenta evitar colidir com os inimigos.*

## Milestone 10 - Isometria - To be implemented later
* IsometricCamera
* Sistema em Grid
* Isometric Depth Sorting
* Tilemaps

*Representar ambientes isométricos respeitando profundidade dos objetos.*

## Milestone 11 - Demo 3d
* Projeção de perspetiva
* Camera3D
* Modelos 3D
* Iluminação

*Implementar um mundo básico em 3 dimensões.*

## Milestone 12 - Gestão de recursos: Serialização
* Serialização
