import {useEffect, useState} from "react";

const MousePosition = ({render}) => {
    const [mousePosition, setMousePosition] = useState({
        x: 0,
        y: 0,
    });

    useEffect(() => {
        const updateMousePosition = (e) => {
            setMousePosition({x: e.clientX, y: e.clientY});
        };
        window.addEventListener("mousemove", updateMousePosition);

        return () => window.removeEventListener("mousemove", updateMousePosition);
    }, []);

    return render({mousePosition});
};

const PanelMouseLogger = () {
    return (
        <div className="BasicTracker">
            <p>Mouse position:</p>
            <MousePosition
                render={({mousePosition}) => (
                    <div className="Row">
                        <span>x: {mousePosition.x}</span>
                        <span>y: {mousePosition.y}</span>
                    </div>
                )}
            />
        </div>
    );
};

const PointMouseLogger = () => {
    return (
        <MousePosition
            render={({mousePosition}) => (
                <p>
                    ({mousePosition.x}, {mousePosition.y})
                </p>
            )}
        />
    );

};

function App() {
    return (
        <div className="App">
            <header className="Header">Header</header>
            <PanelMouseLogger />
            <PointMouseLogger />
        </div>
    );
}

export default App;