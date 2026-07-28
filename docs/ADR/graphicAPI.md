# Escolha de backend gráfico - OpenGL

## Contexto

É necessário de utilizar uma API gráfica para renderização.

Os objetivos deste projeto são:

* A aprendizagem acerca de desenvolvimento de game engines
* Renderização 2D e isométrica e potencial renderização 3D
* Portabilidade 
* Desenvolvimento num prazo limitado e curto 

## Decisão 

Escolhi como backend gráfico inicial o OpenGL.
Outra alternativa possível será o uso da API Vulkan.

## Justificação

### Simplicidade

O OpenGL requer uma curva de aprendizagem inferior comparado com Vulkan.

### Tempo

Como o objetivo principal nesta fase é ter desenvolvido um motor funcional, a complexidade ao utilizar Vulkan aumenta o custo de tempo no sua aprendizagem e desenvolvimento para alcançar os mesmos resultados pretendidos.

### Adequamento ao escopo

O foco atual é a renderização em 2D e isométrica, as quais não são justificáveis implementar backend com Vulkan.

## Implicações

### Consequências positivas

* Redução na complexidade inicial e maior produtividade
* Baixa curva de aprendizagem
* Relocação de tempo para construção da arquitetura do motor

### Consequências negativas

* Menor controlo low-level, levando a menor potencial para otimização 

## Considerações futuras 

Será necessário manter separado o Renderer e o Backend gráfico, assim permitindo a implementação da API Vulkan sem alterar o núcleo do motor.

## Tabela comparativa

| Critério | OpenGL | Vulkan |
|---|---|---|
| Curva de aprendizagem | Baixa | Alta |
| Complexidade | Baixa | Alta |
| Desenvolvimento | Adequado | Exigente |
| Renderização | Adequado | Adequado |
| Custo de tempo | Normal | Elevado |
| Performance / Otimização | Fraca | Forte |
