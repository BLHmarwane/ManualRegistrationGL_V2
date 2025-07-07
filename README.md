# Manual Registration Simulator V2

A modern QML-based 3D manual registration simulator designed for research applications. This tool allows researchers to study human interaction with 3D objects through two main modes: Ground Truth mode for controlled training with predefined shapes (cube, sphere, torus, tetrahedron), and Stereo Image mode for real-world scenarios using user-provided stereo image pairs with .obj/.stl model overlays. Built with Qt6 QML architecture, the simulator supports multiple input devices including SpaceMouse for 6DOF manipulation, multi-touch gestures, and traditional mouse/keyboard input. The application features a responsive Material Design interface with smooth animations, comprehensive data logging for research analysis, and modular QML components that can be easily integrated into other applications. This represents a complete architectural redesign from the original Qt Widgets version, offering better performance, modern touch-first interaction, and professional-grade research capabilities for studying manual registration techniques, multi-modal input methods, and 3D user interface design.

## Features
- **Ground Truth Mode**: Controlled training environment with predefined 3D shapes
- **Stereo Image Mode**: Real-world registration using stereo image pairs
- **SpaceMouse Support**: Professional 6DOF manipulation device integration
- **Multi-Touch Gestures**: Modern touch-first interface design
- **Traditional Input**: Mouse/keyboard fallback for universal accessibility
- **QML Architecture**: Responsive, modular components for easy integration
- **Data Logging**: Comprehensive research-grade interaction recording
- **Material Design**: Professional, accessible user interface

## Technology Stack
Built with Qt6 QML + OpenCV + OpenGL for modern 3D registration research.

## Quick Start
```bash
git clone https://github.com/BLHmarwane/ManualRegistrationGL_V2.git
cd ManualRegistrationGL_V2
./scripts/install_dependencies.sh
mkdir build && cd build && cmake .. && make
./ManualRegistrationGL_V2
```
### If you use this simulator in your research, please cite:
```bibtex
@software{manual_registration_v2,
  title={Manual Registration Simulator V2: QML-based 3D Registration Research Tool},
  author={BELHAMRA Marwane},
  year={2025},
  url={https://github.com/BLHmarwane/ManualRegistrationGL_V2}
}
```
## Built with ❤️ for the research community