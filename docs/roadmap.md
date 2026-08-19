# Roadmap

## Milestone inicial - Base para desenvolvimento - done
* Estrutura inicial do projeto
* Configuração de build com CMake
* openGL context 
* Processamento de eventos com SDL3 - 50% done

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
* Camera2D - Dependente da milestone 3

*Tornar o motor capaz de renderizar texturas/sprites 2D no ecrã.*

## Milestone 3 - Matemática
* Vector2D, Vector3D
* Transform
* Matrix4x4 - Identity
* Matrix4x4 - Translation
* Matrix4x4 - Scale
* Matrix4x4 - Multiplication
* Matrix4x4 - Ortographic projection

## Milestone 4 - Isometria
* IsometricCamera
* Sistema em Grid
* Isometric Depth Sorting
* Tilemaps

*Representar ambientes isométricos respeitando profundidade dos objetos.*

## Milestone 5 - Gestão de recursos
* Asset manager
* Scene manager
* Serialização

*Tornar o motor capaz de carregar/alternar entre diferentes cenas.*

## Milestone 6 - ECS 
* Entities
* Components
* Systems 

*Arquitetura baseada em entidades e componentes, assim evitando usar hierarquia em classes.*

## Em desenvolvimento
