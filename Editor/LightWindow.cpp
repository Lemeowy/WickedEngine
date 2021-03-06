#include "stdafx.h"
#include "LightWindow.h"


LightWindow::LightWindow(wiGUI* gui) : GUI(gui), light(nullptr)
{
	assert(GUI && "Invalid GUI!");

	float screenW = (float)wiRenderer::GetDevice()->GetScreenWidth();
	float screenH = (float)wiRenderer::GetDevice()->GetScreenHeight();

	lightWindow = new wiWindow(GUI, "Light Window");
	lightWindow->SetSize(XMFLOAT2(650, 500));
	//lightWindow->SetEnabled(false);
	GUI->AddWidget(lightWindow);

	float x = 450;
	float y = 0;
	float step = 35;

	energySlider = new wiSlider(0.1f, 64, 0, 100000, "Energy: ");
	energySlider->SetSize(XMFLOAT2(100, 30));
	energySlider->SetPos(XMFLOAT2(x, y += step));
	energySlider->OnSlide([&](wiEventArgs args) {
		if (light != nullptr)
		{
			light->enerDis.x = args.fValue;
		}
	});
	energySlider->SetEnabled(false);
	energySlider->SetTooltip("Adjust the light radiation amount inside the maximum range");
	lightWindow->AddWidget(energySlider);

	distanceSlider = new wiSlider(1, 1000, 0, 100000, "Distance: ");
	distanceSlider->SetSize(XMFLOAT2(100, 30));
	distanceSlider->SetPos(XMFLOAT2(x, y += step));
	distanceSlider->OnSlide([&](wiEventArgs args) {
		if (light != nullptr)
		{
			light->enerDis.y = args.fValue;
		}
	});
	distanceSlider->SetEnabled(false);
	distanceSlider->SetTooltip("Adjust the maximum range the light can affect.");
	lightWindow->AddWidget(distanceSlider);

	radiusSlider = new wiSlider(0.01f, 10, 0, 100000, "Radius: ");
	radiusSlider->SetSize(XMFLOAT2(100, 30));
	radiusSlider->SetPos(XMFLOAT2(x, y += step));
	radiusSlider->OnSlide([&](wiEventArgs args) {
		if (light != nullptr)
		{
			light->radius = args.fValue;
		}
	});
	radiusSlider->SetEnabled(false);
	radiusSlider->SetTooltip("Adjust the radius of an area light.");
	lightWindow->AddWidget(radiusSlider);

	widthSlider = new wiSlider(1, 10, 0, 100000, "Width: ");
	widthSlider->SetSize(XMFLOAT2(100, 30));
	widthSlider->SetPos(XMFLOAT2(x, y += step));
	widthSlider->OnSlide([&](wiEventArgs args) {
		if (light != nullptr)
		{
			light->width = args.fValue;
		}
	});
	widthSlider->SetEnabled(false);
	widthSlider->SetTooltip("Adjust the width of an area light.");
	lightWindow->AddWidget(widthSlider);

	heightSlider = new wiSlider(1, 10, 0, 100000, "Height: ");
	heightSlider->SetSize(XMFLOAT2(100, 30));
	heightSlider->SetPos(XMFLOAT2(x, y += step));
	heightSlider->OnSlide([&](wiEventArgs args) {
		if (light != nullptr)
		{
			light->height = args.fValue;
		}
	});
	heightSlider->SetEnabled(false);
	heightSlider->SetTooltip("Adjust the height of an area light.");
	lightWindow->AddWidget(heightSlider);

	fovSlider = new wiSlider(0.1f, XM_PI - 0.01f, 0, 100000, "FOV: ");
	fovSlider->SetSize(XMFLOAT2(100, 30));
	fovSlider->SetPos(XMFLOAT2(x, y += step));
	fovSlider->OnSlide([&](wiEventArgs args) {
		if (light != nullptr)
		{
			light->enerDis.z = args.fValue;
		}
	});
	fovSlider->SetEnabled(false);
	fovSlider->SetTooltip("Adjust the cone aperture for spotlight.");
	lightWindow->AddWidget(fovSlider);

	biasSlider = new wiSlider(0.0f, 0.2f, 0, 100000, "ShadowBias: ");
	biasSlider->SetSize(XMFLOAT2(100, 30));
	biasSlider->SetPos(XMFLOAT2(x, y += step));
	biasSlider->OnSlide([&](wiEventArgs args) {
		if (light != nullptr)
		{
			light->shadowBias = args.fValue;
		}
	});
	biasSlider->SetEnabled(false);
	biasSlider->SetTooltip("Adjust the shadow bias if shadow artifacts occur.");
	lightWindow->AddWidget(biasSlider);

	shadowCheckBox = new wiCheckBox("Shadow: ");
	shadowCheckBox->SetPos(XMFLOAT2(x, y += step));
	shadowCheckBox->OnClick([&](wiEventArgs args) {
		if (light != nullptr)
		{
			light->shadow = args.bValue;
		}
	});
	shadowCheckBox->SetEnabled(false);
	shadowCheckBox->SetTooltip("Set light as shadow caster. Many shadow casters can affect performance!");
	lightWindow->AddWidget(shadowCheckBox);

	volumetricsCheckBox = new wiCheckBox("Volumetric Scattering: ");
	volumetricsCheckBox->SetPos(XMFLOAT2(x, y += step));
	volumetricsCheckBox->OnClick([&](wiEventArgs args) {
		if (light != nullptr)
		{
			light->volumetrics = args.bValue;
		}
	});
	volumetricsCheckBox->SetEnabled(false);
	volumetricsCheckBox->SetTooltip("Compute volumetric light scattering effect. The scattering is modulated by fog settings!");
	lightWindow->AddWidget(volumetricsCheckBox);

	haloCheckBox = new wiCheckBox("Visualizer: ");
	haloCheckBox->SetPos(XMFLOAT2(x, y += step));
	haloCheckBox->OnClick([&](wiEventArgs args) {
		if (light != nullptr)
		{
			light->noHalo = !args.bValue;
		}
	});
	haloCheckBox->SetEnabled(false);
	haloCheckBox->SetTooltip("Visualize light source emission");
	lightWindow->AddWidget(haloCheckBox);

	addLightButton = new wiButton("Add Light");
	addLightButton->SetPos(XMFLOAT2(x, y += step));
	addLightButton->SetSize(XMFLOAT2(150, 30));
	addLightButton->OnClick([&](wiEventArgs args) {
		Light* light = new Light;
		light->enerDis = XMFLOAT4(2, 60, XM_PIDIV4, 0);
		light->color = XMFLOAT4(1, 1, 1, 1);
		light->Translate(XMFLOAT3(0, 3, 0));
		light->SetType(Light::POINT);
		wiRenderer::Add(light);
	});
	addLightButton->SetTooltip("Add a light to the scene.");
	lightWindow->AddWidget(addLightButton);


	colorPicker = new wiColorPicker(GUI, "Light Color");
	colorPicker->SetPos(XMFLOAT2(10, 30));
	colorPicker->RemoveWidgets();
	colorPicker->SetVisible(true);
	colorPicker->SetEnabled(true);
	colorPicker->OnColorChanged([&](wiEventArgs args) {
		if(light!=nullptr)
			light->color = XMFLOAT4(powf(args.color.x, 1.f / 2.2f), powf(args.color.y, 1.f / 2.2f), powf(args.color.z, 1.f / 2.2f), 1);
	});
	lightWindow->AddWidget(colorPicker);

	typeSelectorComboBox = new wiComboBox("Type: ");
	typeSelectorComboBox->SetPos(XMFLOAT2(x, y += step));
	typeSelectorComboBox->OnSelect([&](wiEventArgs args) {
		if (light != nullptr && args.iValue >= 0)
		{
			light->SetType((Light::LightType)args.iValue);
			SetLightType(light->GetType());
			biasSlider->SetValue(light->shadowBias);
		}
	});
	typeSelectorComboBox->SetEnabled(false);
	typeSelectorComboBox->AddItem("Directional");
	typeSelectorComboBox->AddItem("Point");
	typeSelectorComboBox->AddItem("Spot");
	typeSelectorComboBox->AddItem("Sphere");
	typeSelectorComboBox->AddItem("Disc");
	typeSelectorComboBox->AddItem("Rectangle");
	typeSelectorComboBox->AddItem("Tube");
	typeSelectorComboBox->SetTooltip("Choose the light source type...");
	lightWindow->AddWidget(typeSelectorComboBox);


	lightWindow->Translate(XMFLOAT3(30, 30, 0));
	lightWindow->SetVisible(false);

	SetLight(nullptr);
}


LightWindow::~LightWindow()
{
	lightWindow->RemoveWidgets(true);
	GUI->RemoveWidget(lightWindow);
	SAFE_DELETE(lightWindow);
}

void LightWindow::SetLight(Light* light)
{
	if (this->light == light)
		return;

	this->light = light;
	if (light != nullptr)
	{
		//lightWindow->SetEnabled(true);
		energySlider->SetEnabled(true);
		energySlider->SetValue(light->enerDis.x);
		distanceSlider->SetValue(light->enerDis.y);
		radiusSlider->SetValue(light->radius);
		widthSlider->SetValue(light->width);
		heightSlider->SetValue(light->height);
		fovSlider->SetValue(light->enerDis.z);
		biasSlider->SetEnabled(true);
		biasSlider->SetValue(light->shadowBias);
		shadowCheckBox->SetEnabled(true);
		shadowCheckBox->SetCheck(light->shadow);
		haloCheckBox->SetEnabled(true);
		haloCheckBox->SetCheck(!light->noHalo);
		volumetricsCheckBox->SetEnabled(true);
		volumetricsCheckBox->SetCheck(light->volumetrics);
		colorPicker->SetEnabled(true);
		typeSelectorComboBox->SetEnabled(true);
		typeSelectorComboBox->SetSelected((int)light->GetType());
		
		SetLightType(light->GetType());
	}
	else
	{
		distanceSlider->SetEnabled(false);
		radiusSlider->SetEnabled(false);
		widthSlider->SetEnabled(false);
		heightSlider->SetEnabled(false);
		fovSlider->SetEnabled(false);
		biasSlider->SetEnabled(false);
		shadowCheckBox->SetEnabled(false);
		haloCheckBox->SetEnabled(false);
		volumetricsCheckBox->SetEnabled(false);
		energySlider->SetEnabled(false);
		colorPicker->SetEnabled(false);
		typeSelectorComboBox->SetEnabled(false);
		//lightWindow->SetEnabled(false);
	}
}
void LightWindow::SetLightType(Light::LightType type)
{
	if (type == Light::DIRECTIONAL)
	{
		distanceSlider->SetEnabled(false);
		fovSlider->SetEnabled(false);
	}
	else
	{
		if (type == Light::SPHERE || type == Light::DISC || type == Light::RECTANGLE || type == Light::TUBE)
		{
			distanceSlider->SetEnabled(false);
			radiusSlider->SetEnabled(true);
			widthSlider->SetEnabled(true);
			heightSlider->SetEnabled(true);
			fovSlider->SetEnabled(false);
		}
		else
		{
			distanceSlider->SetEnabled(true);
			radiusSlider->SetEnabled(false);
			widthSlider->SetEnabled(false);
			heightSlider->SetEnabled(false);
			if (type == Light::SPOT)
			{
				fovSlider->SetEnabled(true);
			}
			else
			{
				fovSlider->SetEnabled(false);
			}
		}
	}

}
