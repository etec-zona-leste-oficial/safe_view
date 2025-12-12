# Projeto de Conclusão de Curso | Sistema IoT para diminuir os impactos de Pontos Cegos
 
🎯 Temos como objetivo geral desenvolver e implementar um dispositivo IoT constituído por câmeras e sensores, integrado a um aplicativo capaz de transmitir os dados ao motorista, permitindo o monitoramento acerca do veículo.

🧑‍🔧 Temos como alvo o âmbito do setor de transportes, marcado por demandas crescentes e condições desafiadoras, buscamos atingir empresas de pequeno porte e motoristas autônomos que priorizam sua segurança e a de terceiros.

<div align="center">
 <p>Lembre-se: SafeView - <Strong>revelando o que os olhos não veem</Strong></p>

<img width="250" height="500" alt="2" src="https://github.com/user-attachments/assets/95e20207-5433-4f7f-bb5c-783fdb92dce5" />
</div>


# 🛠️ Tecnologias Utilizadas
Hardware

- Sensores Ultrassônicos
- ESP32 / ESP32-CAM
- Raspberry Pi

Software & Ferramentas

- Python
- C++
- React — 19.1.0 | React-native — 0.81.5
- JavaScript
- Firebase
- Figma
- UML
<br>

## 🤝Como contribuir
Realizamos uma pesquisa de campo e ao conversar com motoristas e ver a rotina real, percebemos ainda mais essa dificuldade. Esses relatos mostraram que o problema é constante e afeta tanto motoristas experientes quanto iniciantes, reforçando ainda mais a necessidade de uma solução prática e acessível.
Se você já passou por algo parecido, compartilhe sua experiência no nosso formulário: 🔗https://forms.gle/WmKFNv2wfDB3YCT16
<br>

## ▶️ Como Executar o Projeto ?
1. Requisitos

Antes de iniciar, certifique-se de que seu ambiente possui:

- Node.js 20 (recomendado: nvm use 20.19.5)
- npm 10+
- Java 17+
- Expo SDK 54
- NVM instalado (para gerenciar versões do Node)

Ambiente configurado para:

- Firebase
- Sensores e módulos ESP32 / ESP32-CAM
- Raspberry Pi

2. Verificações Iniciais

Antes de rodar o projeto:

- node -v
- npm -v

(Opcional) Atualizar Expo para evitar conflitos
- npm install -g expo
- npm update expo
- npx expo start -c
  
3. Executando o Projeto
- git clone <URL_DO_REPOSITORIO>
- npm install
- npx expo install
- npx expo start -c

4. Criar arquivo .nvmrc

O Expo ainda não oferece suporte total ao Node 22. Caso seu sistema atualize automaticamente, siga os passos:

Na raiz do projeto, crie um arquivo chamado .nvmrc contendo: 20
Forçar uso da versão correta
- nvm use

Reinstalar dependências
- rm -rf node_modules package-lock.json
- npm install

Se ocorrer erro, instale o NVM para Windows:
https://github.com/coreybutler/nvm-windows/releases


5. Configurações Complementares

Após concluir os passos acima, configure:

- Integração com sensores
- Comunicação com módulos ESP32 / ESP32-CAM
- Módulo Raspberry Pi
  
<!-- ADD SOBRE O IOT (É BASICAMENTE UM RESUMO DA DOCUMENTAÇÃO DOS COMPONENTES ELETRONICOS SÓ QUE AGORA PARA FAZE-LO FUNCIONAR INTEGRADO COM O APP) !-->

# 📄 Licenciamento e creditamento

👥 Integrantes do Grupo

<div align="center">
<img width="500" height="862" alt="image" src="https://github.com/user-attachments/assets/7e5a7403-5310-46b7-bc09-ab3451b2d384" />
</div>
<br>

[![Jhonata Alves do Nascimento](https://img.shields.io/badge/Jhonata_Alves_do_Nascimento-0d1b2a?style=for-the-badge&labelColor=004b8d&color=0d1b2a)](https://github.com/JHOW-JHOW)
[![Ester Rodrigues Soares](https://img.shields.io/badge/Ester_Rodrigues_Soares-0d1b2a?style=for-the-badge&labelColor=004b8d&color=0d1b2a)](https://github.com/EsterRS7)
[![Gabrielly Nascimento Bento](https://img.shields.io/badge/Gabrielly_Nascimento_Bento-0d1b2a?style=for-the-badge&labelColor=004b8d&color=0d1b2a)](https://github.com/Gabr1ell1)
[![Gustavo Henrique Ribeiro da Silva](https://img.shields.io/badge/Gustavo_Henrique_Ribeiro_da_Silva-0d1b2a?style=for-the-badge&labelColor=004b8d&color=0d1b2a)](https://github.com/Guzhie)



> [!IMPORTANT]
> A pasta Application_SafeView não inclui o diretório node_modules, pois ele é gerado automaticamente e possui tamanho elevado. Para utilizá-la corretamente, é necessário instalar todas as dependências seguindo as instruções fornecidas acima.

<!--# ⚠️ Notas Técnicas
A pasta Application_SafeView não inclui o diretório node_modules, pois ele é gerado automaticamente e possui tamanho elevado. Para utilizá-la corretamente, é necessário instalar todas as dependências seguindo as instruções fornecidas acima.

> [!NOTE]
> Useful information that users should know, even when skimming content.

> [!TIP]
> Helpful advice for doing things better or more easily.

> [!WARNING]
> Urgent info that needs immediate user attention to avoid problems.



































