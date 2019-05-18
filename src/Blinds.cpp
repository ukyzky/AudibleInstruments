#include "AudibleInstruments.hpp"


struct Blinds : Module {
	enum ParamIds {
		GAIN1_PARAM,
		GAIN2_PARAM,
		GAIN3_PARAM,
		GAIN4_PARAM,
		MOD1_PARAM,
		MOD2_PARAM,
		MOD3_PARAM,
		MOD4_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		IN1_INPUT,
		IN2_INPUT,
		IN3_INPUT,
		IN4_INPUT,
		CV1_INPUT,
		CV2_INPUT,
		CV3_INPUT,
		CV4_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUT1_OUTPUT,
		OUT2_OUTPUT,
		OUT3_OUTPUT,
		OUT4_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		CV1_POS_LIGHT, CV1_NEG_LIGHT,
		CV2_POS_LIGHT, CV2_NEG_LIGHT,
		CV3_POS_LIGHT, CV3_NEG_LIGHT,
		CV4_POS_LIGHT, CV4_NEG_LIGHT,
		OUT1_POS_LIGHT, OUT1_NEG_LIGHT,
		OUT2_POS_LIGHT, OUT2_NEG_LIGHT,
		OUT3_POS_LIGHT, OUT3_NEG_LIGHT,
		OUT4_POS_LIGHT, OUT4_NEG_LIGHT,
		NUM_LIGHTS
	};

	Blinds() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
	void step() override;
};


void Blinds::step() {
	float out = 0.0;

	for (int i = 0; i < 4; i++) {
		float g = params[GAIN1_PARAM + i].value;
		g += params[MOD1_PARAM + i].value * inputs[CV1_INPUT + i].value / 5.0;
		g = clamp(g, -2.0f, 2.0f);
		lights[CV1_POS_LIGHT + 2*i].setBrightnessSmooth(fmaxf(0.0, g));
		lights[CV1_NEG_LIGHT + 2*i].setBrightnessSmooth(fmaxf(0.0, -g));
		out += g * inputs[IN1_INPUT + i].normalize(5.0);
		lights[OUT1_POS_LIGHT + 2*i].setBrightnessSmooth(fmaxf(0.0, out / 5.0));
		lights[OUT1_NEG_LIGHT + 2*i].setBrightnessSmooth(fmaxf(0.0, -out / 5.0));
		if (outputs[OUT1_OUTPUT + i].active) {
			outputs[OUT1_OUTPUT + i].value = out;
			out = 0.0;
		}
	}
}


struct BlindsWidget : ModuleWidget {
	BlindsWidget(Blinds *module) : ModuleWidget(module) {
		setPanel(SVG::load(assetPlugin(pluginInstance, "res/Blinds.svg")));

		addChild(createWidget<ScrewSilver>(Vec(15, 0)));
		addChild(createWidget<ScrewSilver>(Vec(150, 0)));
		addChild(createWidget<ScrewSilver>(Vec(15, 365)));
		addChild(createWidget<ScrewSilver>(Vec(150, 365)));

		addParam(createParam<Rogan1PSWhite>(Vec(8, 52), module, Blinds::GAIN1_PARAM, -1.0, 1.0, 0.0));
		addParam(createParam<Rogan1PSWhite>(Vec(8, 131), module, Blinds::GAIN2_PARAM, -1.0, 1.0, 0.0));
		addParam(createParam<Rogan1PSWhite>(Vec(8, 210), module, Blinds::GAIN3_PARAM, -1.0, 1.0, 0.0));
		addParam(createParam<Rogan1PSWhite>(Vec(8, 288), module, Blinds::GAIN4_PARAM, -1.0, 1.0, 0.0));

		addParam(createParam<Trimpot>(Vec(72, 63), module, Blinds::MOD1_PARAM, -1.0, 1.0, 0.0));
		addParam(createParam<Trimpot>(Vec(72, 142), module, Blinds::MOD2_PARAM, -1.0, 1.0, 0.0));
		addParam(createParam<Trimpot>(Vec(72, 221), module, Blinds::MOD3_PARAM, -1.0, 1.0, 0.0));
		addParam(createParam<Trimpot>(Vec(72, 300), module, Blinds::MOD4_PARAM, -1.0, 1.0, 0.0));

		addInput(createPort<PJ301MPort>(Vec(110, 41), PortWidget::INPUT, module, Blinds::IN1_INPUT));
		addInput(createPort<PJ301MPort>(Vec(110, 120), PortWidget::INPUT, module, Blinds::IN2_INPUT));
		addInput(createPort<PJ301MPort>(Vec(110, 198), PortWidget::INPUT, module, Blinds::IN3_INPUT));
		addInput(createPort<PJ301MPort>(Vec(110, 277), PortWidget::INPUT, module, Blinds::IN4_INPUT));

		addInput(createPort<PJ301MPort>(Vec(110, 80), PortWidget::INPUT, module, Blinds::CV1_INPUT));
		addInput(createPort<PJ301MPort>(Vec(110, 159), PortWidget::INPUT, module, Blinds::CV2_INPUT));
		addInput(createPort<PJ301MPort>(Vec(110, 238), PortWidget::INPUT, module, Blinds::CV3_INPUT));
		addInput(createPort<PJ301MPort>(Vec(110, 316), PortWidget::INPUT, module, Blinds::CV4_INPUT));

		addOutput(createPort<PJ301MPort>(Vec(144, 41), PortWidget::OUTPUT, module, Blinds::OUT1_OUTPUT));
		addOutput(createPort<PJ301MPort>(Vec(144, 120), PortWidget::OUTPUT, module, Blinds::OUT2_OUTPUT));
		addOutput(createPort<PJ301MPort>(Vec(144, 198), PortWidget::OUTPUT, module, Blinds::OUT3_OUTPUT));
		addOutput(createPort<PJ301MPort>(Vec(144, 277), PortWidget::OUTPUT, module, Blinds::OUT4_OUTPUT));

		addChild(createLight<SmallLight<GreenRedLight>>(Vec(78, 96), module, Blinds::CV1_POS_LIGHT));
		addChild(createLight<SmallLight<GreenRedLight>>(Vec(78, 175), module, Blinds::CV2_POS_LIGHT));
		addChild(createLight<SmallLight<GreenRedLight>>(Vec(78, 254), module, Blinds::CV3_POS_LIGHT));
		addChild(createLight<SmallLight<GreenRedLight>>(Vec(78, 333), module, Blinds::CV4_POS_LIGHT));

		addChild(createLight<MediumLight<GreenRedLight>>(Vec(152, 87), module, Blinds::OUT1_POS_LIGHT));
		addChild(createLight<MediumLight<GreenRedLight>>(Vec(152, 166), module, Blinds::OUT2_POS_LIGHT));
		addChild(createLight<MediumLight<GreenRedLight>>(Vec(152, 245), module, Blinds::OUT3_POS_LIGHT));
		addChild(createLight<MediumLight<GreenRedLight>>(Vec(152, 324), module, Blinds::OUT4_POS_LIGHT));
	}
};


Model *modelBlinds = createModel<Blinds, BlindsWidget>("Blinds");
